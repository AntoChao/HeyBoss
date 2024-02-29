// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// perception headers
#include "GameFramework/Controller.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

// navigation
#include "NavigationSystem.h"

#include "../../Weapons/WeaponClass.h"

AEnemyClass::AEnemyClass() {
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	
	InteractCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("InteractComp"));
	InteractCollisionComp->BodyInstance.SetCollisionProfileName("InteractComp");
	InteractCollisionComp->SetupAttachment(GetMesh());
}

void AEnemyClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	SetupStimulus();
	InitializeProperties();
	PlayIntroMontage();
}

void AEnemyClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

/* Setups*/
void AEnemyClass::SetupStimulus() {
	if (IsValid(Stimulus)) {
		Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
		Stimulus->RegisterWithPerceptionSystem();
	}
}

void AEnemyClass::InitializeProperties() {
	InteractCollisionComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEnemyClass::DetecPlayerOverlap);

	Player = Cast<APlayerClass>(GetWorld()->GetFirstPlayerController()->GetPawn());
	EnemyController = Cast<AEnemyController>(GetController());

	CollisionParams.AddIgnoredActor(this); // Ignore the character itself

	if (IsValid(Player)) {
		NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Player);
	}
}

/* Take Damage*/
bool AEnemyClass::IsInstigatorValid(AController* EventInstigator) {
	ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(EventInstigator);
	
	if (IsValid(PlayerController)) {
		return true;
	}
	else {
		return false;
	}
}

/* Intro Animation*/
void AEnemyClass::PlayIntroMontage() {
	EnemyController->SetIntroFinish(true);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance)) {
		if (IsValid(Intro_Montage)) {
			EnemyController->SetIntroFinish(false);

			AnimInstance->Montage_Play(Intro_Montage, AnimMontagePlayRate);

			AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &AEnemyClass::IntroEndEffect);
		}
	}
}

void AEnemyClass::IntroEndEffect(UAnimMontage* Montage, bool bInterrupted) {
	EnemyController->SetIntroFinish(true);
}


/* Interaction with Player*/
void AEnemyClass::DetecPlayerOverlap(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (CanDetectPlayer) {
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
			APlayerClass* PlayerDetected = Cast<APlayerClass>(OtherActor);
			if (IsValid(PlayerDetected)) {
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Enemy is detecing player"));

				PlayerDetected->EnableInteraction(this);
			}
		}
	}
}

/* Custom Move*/
void AEnemyClass::WrapperMoveToAttackOffset(FVector Location) {
	
	DrawDebugPoint(GetWorld(), Location,
		10.0f, FColor::Yellow, false, 10.0f);

	bLookingAtPlayer = true;
	EnemyController->SetActionRequireAttackOffset(true);
	EnemyController->SetAttackOffset(Location);
	FinishAction();
}
void AEnemyClass::MoveToEnd() {
	IsDoingAnyAction = false;
	bLookingAtPlayer = false;
}

/* Actions*/
	// Action Control
FVector AEnemyClass::GetPlayerLocation() {
	return Player->GetActorLocation();
}
float AEnemyClass::GetPlayerDistance() {
	return (Player->GetActorLocation() - GetActorLocation()).Size();

}
FVector AEnemyClass::GetPlayerDirection() {
	return (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
}
FVector AEnemyClass::GetPlayerSameHeightDirection() {
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector::DownVector * 1000.0f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
	
	float ImpactPointHeight = 0.0f;
	if (bHit) {
		ImpactPointHeight = HitResult.ImpactPoint.Z;
	}
	
	FVector PlayerLocation = FVector(Player->GetActorLocation().X, 
		Player->GetActorLocation().Y, BodyHalfHeight + ImpactPointHeight);
	
	/*
	DrawDebugPoint(GetWorld(), PlayerLocation,
		15.0f, FColor::Green, false, 10.0f);
	DrawDebugPoint(GetWorld(), GetActorLocation(),
		15.0f, FColor::Green, false, 10.0f);*/

	return (PlayerLocation - GetActorLocation()).GetSafeNormal();
}

FVector AEnemyClass::GetRandomLocation(FVector IniPosition, float Distance) {
	FVector RandomLocation;
	bool bSuccess = NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), IniPosition, RandomLocation,
		Distance);

	return RandomLocation;
}

bool AEnemyClass::LocationValidForTP(FVector TPLocation) {
	
	FHitResult CheckTPHit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(CheckTPHit, TPLocation,
		TPLocation + FVector(0.0f, 0.0f, 400.0f), ECC_Visibility, CollisionParams);

	return !bHit;
}

int AEnemyClass::GetRandomInRangeNum(int max) {
	return FMath::RandRange(1, max);
}

FVector AEnemyClass::GetBackwardRepositionLocation() {
	FVector MoveToVector = GetActorLocation() + GetPlayerSameHeightDirection() * -1 * RepositionBackDistance;
	return MoveToVector;
}

FVector AEnemyClass::GetForwardRepositionLocation() {
	FVector MoveToVector = GetActorLocation() + GetPlayerSameHeightDirection() * RepositionFrontDistance;
	return MoveToVector;
}

FVector AEnemyClass::GetLeftRepositionLocation() {
	FVector ForwardVector = GetPlayerSameHeightDirection();
	FVector UpVector = GetActorUpVector();

	// Calculate the right vector using the cross product and normalize it
	FVector MoveToVector = FVector::CrossProduct(ForwardVector, UpVector).GetSafeNormal();
	MoveToVector = MoveToVector * -1 * RepositionLeftDistance;

	return MoveToVector;
}

FVector AEnemyClass::GetRightRepositionLocation() {
	FVector ForwardVector = GetPlayerSameHeightDirection();
	FVector UpVector = GetActorUpVector();

	// Calculate the right vector using the cross product and normalize it
	FVector MoveToVector = FVector::CrossProduct(ForwardVector, UpVector).GetSafeNormal();
	MoveToVector = MoveToVector * RepositionRightDistance;
	return MoveToVector;
}

FVector AEnemyClass::GetPlayerFrontPosition() {
	return GetPlayerLocation() + GetPlayerSameHeightDirection() * -1 * CloseMainDistance;
}
FVector AEnemyClass::GetPlayerBackPosition() {
	return GetPlayerLocation() + GetPlayerSameHeightDirection() * CloseMainDistance;
}
	
void AEnemyClass::LookAtPlayer() {
	
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector::UpVector * 1000.0f; 

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams)) {
		
		FRotator TargetRotation = FVector::VectorPlaneProject(GetPlayerSameHeightDirection(), 
			HitResult.Normal).Rotation();

		// Set the actor's rotation
		SetActorRotation(TargetRotation);
	}
}

void AEnemyClass::TurnToPlayer() {
	return;
}

void AEnemyClass::SelectActionToDo() {
	EnemyController->SetActionRequireAttackOffset(false);
	EnemyController->SetRequireExtraMoveOne(false);
	EnemyController->SetRequireExtraMoveTwo(false);

	AceptAction();
}

void AEnemyClass::DoActionWithOffset(int OriginalChance, TFunction<void()> OriginalAction,
	TFunction<void()> SecondaryAction) {

	int CurrentChance = GetRandomInRangeNum(100);

	if (CurrentChance < OriginalChance) {
		OriginalAction();
	}
	else {
		SecondaryAction();
	}
}

void AEnemyClass::AceptAction() {
	EnemyController->SetRegettingAction(false);
}
void AEnemyClass::RegreatAction() {
	FinishAction();
	EnemyController->SetRegettingAction(true);
}

bool AEnemyClass::GetDoingAnyAction() {
	return IsDoingAnyAction;
}

void AEnemyClass::FinishAction() {
	IsDoingAnyAction = false;
}
void AEnemyClass::StopAtLookingPlayer() {
	bLookingAtPlayer = false;
}

	// Override Actions
void AEnemyClass::UnseenRepositionAction() {
	IsDoingAnyAction = true;
}

	// Combine it with unseen
bool AEnemyClass::NeedToUnseenReposition() {
	return GetPlayerDistance() > RangedRange;
}

void AEnemyClass::MeleeAction() {
	IsDoingAnyAction = true;
}
void AEnemyClass::MidRangeAction() {
	IsDoingAnyAction = true;
}
void AEnemyClass::RangedAction() {
	IsDoingAnyAction = true;
}

void AEnemyClass::FollowActionOne() {
	IsDoingAnyAction = true;
}
void AEnemyClass::FollowActionTwo() {
	IsDoingAnyAction = true;
}
// Animation Montage
void AEnemyClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	FinishAction();
}