// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterClass.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

ACharacterClass::ACharacterClass() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(BodyRadius, BodyHalfHeight);

	// Create componenets
	CharacterHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	CharacterPowerComponent = CreateDefaultSubobject<UPowerComponent>(TEXT("Power Comp"));
}

void ACharacterClass::BeginPlay() {
	Super::BeginPlay();

	InitializeAllCharacterStats();

}

void ACharacterClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CustomMoveTimeline.TickTimeline(DeltaTime);
}

/* Character Components*/
void ACharacterClass::InitializeAllCharacterStats() {
	// Set custom tick interval
	// PrimaryActorTick.TickInterval = CustomTickInterval;

	// Stats
	// BodyRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	BodyHalfHeight = GetActorLocation().Z;

	// Components
	SetInitCharacterHealth();
	SetInitCharacterPower();
	SetInitCharacterMovement();
	InitializeCustomMoveStats();
}

/* Health Components*/
void ACharacterClass::SetInitCharacterHealth() {
	if (IsValid(CharacterHealthComponent)) {
		CharacterHealthComponent->InitHealth(CharacterInitHealth);
	}
}

// Take Damage
float ACharacterClass::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser) {

	float DamageCaused = Super::TakeDamage(Damage, DamageEvent,
		EventInstigator, DamageCauser);

	// check if the controller is valid
	if (IsValid(EventInstigator)) {
		if (IsInstigatorValid(EventInstigator)) {
			DamageCaused = -DamageCaused;
			CharacterHealthComponent->UpdateHP(DamageCaused);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TOOK DAMAGE"));

			TakeDamageReaction();

			if (CharacterHealthComponent->GetDied()) {
				DieReaction();
			}
		}
	}
	return DamageCaused;
}

// be change based on player or enemy
bool ACharacterClass::IsInstigatorValid(AController* EventInstigator) {
	return false;
}

bool ACharacterClass::IsAbleToTakeDamage() {
	return true;
}

	// Hit reaction
void ACharacterClass::TakeDamageReaction() {
	PlayHitReactionMontage();
}
	 // Spawn Hit Effect
void ACharacterClass::SetHitEffectStats(FVector Location, FRotator Rotation) {
	HitEffectLocation = Location;
	HitEffectRotation = Rotation;
}

void ACharacterClass::SpawnHitEffectNiagara() {
	if (IsValid(HitEffectNiagara)) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffectNiagara,
			HitEffectLocation,
			HitEffectRotation,
			FVector::OneVector,
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
	}
}

	// Hit reaction montage
void ACharacterClass::PlayHitReactionMontage() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance)) {
		if (IsValid(AnimMontage_HitReaction)) {
			AnimInstance->Montage_Play(AnimMontage_HitReaction, 1.0f);
		}
	}
}

	// Die reaction
void ACharacterClass::ActorDie() {
	Destroy();
}

void ACharacterClass::DieReaction() {
	// UnpossessController();
	PlayDieMontage();
}

	// Unposses controller
void ACharacterClass::UnpossessController() {
	AController* ThisController = GetController();

	if (ThisController) {
		ThisController->UnPossess();
	}
}
	// Die reaction montage
void ACharacterClass::PlayDieMontage() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance)) {
		if (IsValid(AnimMontage_Die)) {
			AnimInstance->Montage_Play(AnimMontage_Die, 1.0f);

			AnimInstance->OnMontageEnded.AddUniqueDynamic(this, 
				&ACharacterClass::OnDieMontageEnded);
		}
	}
}

/*
End game if is main
Next Boss or End game if is enemy
*/
void ACharacterClass::OnDieMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	return;
}

/* Power Components*/
void ACharacterClass::SetInitCharacterPower() {
	if (IsValid(CharacterPowerComponent)) {
		CharacterPowerComponent->SetPower(CharacterInitPower);
		CharacterPowerComponent->SetCritChance(CharacterInitCritChance);
		CharacterPowerComponent->SetCritMultiplier(CharacterInitCritMult);
	}
}

UPowerComponent* ACharacterClass::GetCharacterPower() {
	return CharacterPowerComponent;
}

/* Movement Control*/
void ACharacterClass::SetInitCharacterMovement() {
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationRate, 0.0f);
	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->MinAnalogWalkSpeed = MinWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	CurrentMoveSpeed = DefaultMoveSpeed;
}

void ACharacterClass::ChangeCurrentSpeed() {
	float CurrentSpeed = CurrentMoveSpeed + CurrentRunSpeed + CurrentSpeedBoost;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

// Move Speed
void ACharacterClass::ResetCurrentMoveSpeed() {
	CurrentMoveSpeed = DefaultMoveSpeed;
	ChangeCurrentSpeed();
}
void ACharacterClass::UpdateMoveSpeedRowValue(float value) {
	float deltaSpeed = CurrentMoveSpeed + value;
	if (deltaSpeed >= MinWalkSpeed) {
		CurrentMoveSpeed = deltaSpeed;
	}
	else {
		CurrentMoveSpeed = MinWalkSpeed;
	}
	ChangeCurrentSpeed();
}
void ACharacterClass::UpdateMoveSpeedByPercentage(float percentage) {
	float deltaSpeed = CurrentMoveSpeed + CurrentMoveSpeed * percentage;
	if (deltaSpeed >= MinWalkSpeed) {
		CurrentMoveSpeed = deltaSpeed;
	}
	else {
		CurrentMoveSpeed = MinWalkSpeed;
	}
	ChangeCurrentSpeed();
}

// Run Speed
void ACharacterClass::ResetCurrentRunSpeed() {
	CurrentRunSpeed = DefaultRunSpeed;
	ChangeCurrentSpeed();
}
void ACharacterClass::UpdateRunSpeedRowValue(float value) {
	float deltaSpeed = CurrentRunSpeed + value;
	if (deltaSpeed >= 0.0f) {
		CurrentRunSpeed = deltaSpeed;
	}
	else {
		CurrentRunSpeed = 0.0f;
	}
	ChangeCurrentSpeed();
}
void ACharacterClass::UpdateRunSpeedByPercentage(float percentage) {
	float deltaSpeed = CurrentRunSpeed + CurrentMoveSpeed * percentage;
	if (deltaSpeed >= 0.0f) {
		CurrentRunSpeed = deltaSpeed;
	}
	else {
		CurrentRunSpeed = 0.0f;
	}
	ChangeCurrentSpeed();
}

// Speed Boost
void ACharacterClass::ResetCurrentSpeedBoost() {
	CurrentSpeedBoost = DefaultSpeedBoost;
	ChangeCurrentSpeed();
}
void ACharacterClass::UpdateSpeedBoostRowValue(float value, float duration) {
	float deltaSpeed = CurrentSpeedBoost + value;
	if (deltaSpeed >= 0.0f) {
		CurrentSpeedBoost = deltaSpeed;
	}
	else {
		CurrentSpeedBoost = 0.0f;
	}
	ChangeCurrentSpeed();

	GetWorldTimerManager().SetTimer(SpeedBoostTimerHandle, this,
		&ACharacterClass::ResetCurrentSpeedBoost, duration, true);
}

void ACharacterClass::UpdateSpeedBoostByPercentage(float percentage, float duration) {
	float deltaSpeed = CurrentSpeedBoost + CurrentMoveSpeed * percentage;
	if (deltaSpeed >= 0.0f) {
		CurrentSpeedBoost = deltaSpeed;
	}
	else {
		CurrentSpeedBoost = 0.0f;
	}
	ChangeCurrentSpeed();

	GetWorldTimerManager().SetTimer(SpeedBoostTimerHandle, this,
		&ACharacterClass::ResetCurrentSpeedBoost, duration, true);
}

/* Behaviour Control*/
bool ACharacterClass::GetIsAttaking() {
	return bIsAttacking;
}

/* Custom MoveTo*/
void ACharacterClass::InitializeCustomMoveStats() {
	// Setting up CustomMove timeline
	ActorsToIgnore.Add(this);
	DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);

	FOnTimelineFloat CustomMoveTimelineCallBack;
	FOnTimelineEventStatic CustomMoveTimelineFinishedCallBack;

	CustomMoveTimelineCallBack.BindUFunction(this, FName("UpdateCustomMoveLocation"));
	CustomMoveTimeline.AddInterpFloat(CustomMoveCurve, CustomMoveTimelineCallBack);

	CustomMoveTimelineFinishedCallBack.BindUFunction(this, FName("CustomMoveEnd"));
	CustomMoveTimeline.SetTimelineFinishedFunc(CustomMoveTimelineFinishedCallBack);
}

void ACharacterClass::CustomMove(float MoveDuration, FVector TargetLocation) {
	if (IsValid(CustomMoveCurve)) {
		IsCustomMoveing = true;

		CustomMoveInitLocation = GetActorLocation();
		CustomMoveTargetLocation = TargetLocation;

		CustomMoveTimeline.SetTimelineLength(MoveDuration);
		CustomMoveTimeline.SetLooping(false);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CustomMove Begin"));

		CustomMoveTimeline.PlayFromStart();
	}
}

void ACharacterClass::UpdateCustomMoveLocation(float Value) {
	FVector NewLocation = FMath::Lerp(CustomMoveInitLocation, CustomMoveTargetLocation, Value);

	bool bHit1 = GetWorld()->LineTraceSingleByChannel(CustomMoveCollideHit,
		GetActorLocation(), NewLocation + FVector(0.0f, 0.0f, BodyHalfHeight / 2), ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	bool bHit2 = GetWorld()->LineTraceSingleByChannel(CustomMoveCollideHit,
		GetActorLocation(), NewLocation - FVector(0.0f, 0.0f, BodyHalfHeight / 2), ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	bool bHit3 = GetWorld()->LineTraceSingleByChannel(CustomMoveCollideHit,
		GetActorLocation(), NewLocation, ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	if (bHit1 || bHit2 || bHit3) {
		CustomMoveTimeline.Stop();
		CustomMoveEnd();
	}
	else {
		SetActorLocation(NewLocation);
	}
}

void ACharacterClass::CustomMoveEnd() {
	IsCustomMoveing = false;
}

void ACharacterClass::StopCustomMove() {
	CustomMoveTimeline.Stop();
}

// Animation montage
void ACharacterClass::PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance)) {
		if (IsValid(ChosenMontage)) {

			AnimInstance->Montage_Play(ChosenMontage, PlayRate);

			AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &ACharacterClass::OnMontageEnded);
		}
	}
}

void ACharacterClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	return;
}
