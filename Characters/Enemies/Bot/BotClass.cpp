// Copyright Epic Games, Inc. All Rights Reserved.

#include "BotClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


ABotClass::ABotClass() {
	ExplotionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Explotion Range"));
	
	ExplotionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	ExplotionComponent->InitSphereRadius(50.0f);
}

void ABotClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	AttachBodyWeapons();
}

void ABotClass::AttachBodyWeapons() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// left hand
	if (IsValid(LeftHandWeaponClass)) {
		FTransform TransLeftHand = GetMesh()->
			GetSocketTransform(LHSocket, ERelativeTransformSpace::RTS_World);

		LeftHandWeapon = GetWorld()->SpawnActor<AMeleeWeaponClass>(LeftHandWeaponClass,
			TransLeftHand, ActorSpawnParams);

		if (IsValid(LeftHandWeapon)) {
			LeftHandWeapon->SetWeaponController(this);
			LeftHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, LHSocket);

			ActorsToIgnore.Add(LeftHandWeapon);
		}

		DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
	}

	// right hand
	if (IsValid(RightHandWeaponClass)) {
		FTransform TransRightHand = GetMesh()->
			GetSocketTransform(RHSocket, ERelativeTransformSpace::RTS_World);

		RightHandWeapon = GetWorld()->SpawnActor<AMeleeWeaponClass>(RightHandWeaponClass,
			TransRightHand, ActorSpawnParams);

		if (IsValid(RightHandWeapon)) {
			RightHandWeapon->SetWeaponController(this);
			RightHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, RHSocket);

			ActorsToIgnore.Add(RightHandWeapon);
		}

		DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
	}

	// hair
	if (IsValid(HairWeaponClass)) {
		FTransform TransHair = GetMesh()->
			GetSocketTransform(HairSocket, ERelativeTransformSpace::RTS_World);

		HairWeapon = GetWorld()->SpawnActor<AMeleeWeaponClass>(HairWeaponClass,
			TransHair, ActorSpawnParams);

		if (IsValid(HairWeapon)) {
			HairWeapon->SetWeaponController(this);
			HairWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, HairSocket);

			ActorsToIgnore.Add(HairWeapon);
		}

		DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
	}
}

void ABotClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bLookingAtPlayer) {
		LookAtPlayer();
	}
}

void ABotClass::SetIsClone(bool NewAttribute) {
	IsBotClone = NewAttribute;
}

void ABotClass::ActivateWeapon() {
	if (IsValid(LeftHandWeapon)) {
		LeftHandWeapon->Activate(FVector::ZeroVector);
	}
	if (IsValid(RightHandWeapon)) {
		RightHandWeapon->Activate(FVector::ZeroVector);
	}
	if (IsValid(HairWeapon)) {
		HairWeapon->Activate(FVector::ZeroVector);
	}

}

void ABotClass::UpdateDashSpeed(FVector Location) {
	UpdateSpeedBoostRowValue(DashSpeedValue, DashSpeedDuration);
	WrapperMoveToAttackOffset(Location);
}

	// Override Actions
void ABotClass::SelectActionToDo() {
	Super::SelectActionToDo();

	if (NeedToUnseenReposition()) {
		EnemyController->SetCurrentAction(EEnemyAction::EReposition);
	}
	else {
		float Distance = GetPlayerDistance();
		// int OriginalChance;
		if (Distance < MeleeRange) {
			// Melee Attack ->90% melee & 10% mid range
			if (GetRandomInRangeNum(100) < 90) {
				EnemyController->SetCurrentAction(EEnemyAction::EMeleeAction);
			}
			else {
				EnemyController->SetCurrentAction(EEnemyAction::EMidRangeAction);
			}
			
		}
		else if (Distance < MidRange) {
			// MidRange Attack -> 85 / 15
			if (GetRandomInRangeNum(100) < 85) {
				EnemyController->SetCurrentAction(EEnemyAction::EMidRangeAction);
			}
			else {
				EnemyController->SetCurrentAction(EEnemyAction::ERangedAction);
			}
			
		}
		else {
			// Ranged Attack -> 90 / 10
			int Chance = GetRandomInRangeNum(100);
			if (Chance < 90) {
				EnemyController->SetCurrentAction(EEnemyAction::ERangedAction);
			}
			else{
				EnemyController->SetCurrentAction(EEnemyAction::EMidRangeAction);
			}
		}
	}
}

// Reposition
void ABotClass::UnseenRepositionAction() {
	Super::UnseenRepositionAction();
	
	// Get Close to Player
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Move Close To Player"));
	
}

// Melee
void ABotClass::MeleeAction() {
	Super::MeleeAction();

	switch (GetRandomInRangeNum(MeleeActionNum)) {
		case 1: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Single Slash"));
			SingleSlash();
			
			break;
		}
		case 2: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Run Slash"));

			RunSlash();
			break;
		}
		case 3: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Aument Katana"));

			AumentKatana();
			break;
		}
	}
}

	// Activate weapon in animation
void ABotClass::TurnCircleAttack() {
	if (IsValid(TurnCircleAttack_Montage)) {
		PlaySelectedAnimMontage(TurnCircleAttack_Montage, AnimMontagePlayRate);
	}
}

	// TP to a random location around player
void ABotClass::TPAway() {
	if (IsValid(TPAway_Montage)) {
		PlaySelectedAnimMontage(TPAway_Montage, AnimMontagePlayRate);
	}
}
	
void ABotClass::PlayTPEffect() {
	SetActorLocation(GetRandomLocation(GetPlayerLocation(), TPDistance););
}

// Mid Range
void ABotClass::MidRangeAction() {
	Super::MidRangeAction();
	
	switch (GetRandomInRangeNum(MidRangeActionNum)) {
		case 1: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Charge Slash Wave"));
			ChargeWave();

			break;
		}
		case 2: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Charge Dash"));

			ChargeDash();
			break;
		}
		case 3: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Charge Move"));

			LargeChargeDash();

			break;
		}
	}
}


	// Dash Attack -> maybe better custommove + montage
void ABotClass::DashAttack() {
	IsDashing = true;

	FVector DashDestination = GetActorLocation() + GetPlayerSameHeightDirection() * DashAttackDistance;

	ActorsToIgnore.Add(Player);
	DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);

	bool bHit = GetWorld()->LineTraceSingleByChannel(DashCollideHit,
		GetActorLocation(), DashDestination, ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	if (bHit) {
		WrapperMoveToAttackOffset(DashCollideHit.ImpactPoint);
	}
	else {
		WrapperMoveToAttackOffset(DashDestination);
	}
}


	// Spawn two clones to dash to player
void ABotClass::BotCloneAttack() {
	LookAtPlayer();

	if (IsValid(BotCloneAttack_Montage)) {
		PlaySelectedAnimMontage(BotCloneAttack_Montage, AnimMontagePlayRate);
	}
}

void ABotClass::SpawnBotAttack() {
	if (IsValid(BotClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector ForwardVector = GetPlayerSameHeightDirection();
		FVector UpVector = GetActorUpVector();

		// Calculate the right vector using the cross product and normalize it
		FVector RightVector = FVector::CrossProduct(ForwardVector, UpVector).GetSafeNormal();
		FVector RightPosition = RightVector * BotSpawnDistance;
		FRotator RightDirection = (GetPlayerLocation() - RightPosition).Rotation();

		FVector LeftPosition = RightVector * -1 * BotSpawnDistance;
		FRotator LeftDirection = (GetPlayerLocation() - LeftPosition).Rotation();
		

		ABotClass* BotA = GetWorld()->SpawnActor<ABotClass>(
			BotClass, RightPosition, RightDirection,ActorSpawnParams);
		ABotClass* BotB = GetWorld()->SpawnActor<ABotClass>(
			BotClass, LeftPosition, LeftDirection, ActorSpawnParams);

		BotA->SetIsClone(true);
		BotB->SetIsClone(true);
	}
}

void ABotClass::BotArena() {
	if (IsValid(BotArena_Montage)) {
		LookAtPlayer();
		BotArenaCounts = 0;
		PlaySelectedAnimMontage(BotArena_Montage, AnimMontagePlayRate);
	}
}

void ABotClass::SpawnBotArena() {
	if (BotArenaCounts < 10) {

		FVector SpawnLocation = GetRandomLocation(GetPlayerLocation(), 100000.0f);
		while (IsNotOkayToSpawnBotArena(SpawnLocation)) {
			SpawnLocation = GetRandomLocation(GetPlayerLocation(), 100000.0f);
			TrySpawnBotArenaTimes++;
		}

		if (IsValid(BotClass)) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FRotator SpawnDirection = (GetPlayerLocation() - SpawnLocation).Rotation();
			ABotClass* BotC = GetWorld()->SpawnActor<ABotClass>(
				BotClass, SpawnLocation, SpawnDirection, ActorSpawnParams);

			BotC->SetIsClone(true);

			BotArenaCounts++;
			GetWorldTimerManager().SetTimer(BotArenaTimer, this,
				&ABotClass::SpawnBotArena, BotArenaCooldown, false);
		}
	}
}


bool ABotClass::IsNotOkayToSpawnBotArena(FVector SpawnLocation) {
	return (GetPlayerLocation() - SpawnLocation).Size < MinimalSpawnDistnace
		&& TrySpawnBotArenaTimes < 20;
}

void ABotClass::Duplicate() {
	if (IsValid(Duplicate_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(Duplicate_Montage, AnimMontagePlayRate);
	}
}

void ABotClass::SpawnDuplicateBot() {
	if (IsValid(BotClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector RightVector = FVector::CrossProduct(ForwardVector, UpVector).GetSafeNormal();
		FVector RightPosition = RightVector * BotSpawnDistance;
		FRotator RightDirection = (GetPlayerLocation() - RightPosition).Rotation();

		GetWorld()->SpawnActor<ABotClass>(
			BotClass, RightVector, RightDirection, ActorSpawnParams);
	}
}

// Range Action
void ABotClass::RangedAction() {
	Super::RangedAction();

	switch (GetRandomInRangeNum(RangedActionNum)) {
		case 1: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Floating Katana"));

			FlotingKatanas();
			break;
		}
		case 2: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Invoke slash waves"));

			InvokeWaves();
			break;
		}
	}
}

	// Launch MiniBot 
void ABotClass::SpawnMiniBot() {
	if (IsValid(SpawnMiniBot_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(SpawnMiniBot_Montage, AnimMontagePlayRate);
	}
}

void ABotClass::LaunchMiniBot() {
	if (IsValid(MiniBotClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransRightHand = GetMesh()->
			GetSocketTransform(RHSocket, ERelativeTransformSpace::RTS_World);

		GetWorld()->SpawnActor<AMiniBotClass>(MiniBotClass,
			TransRightHand, ActorSpawnParams);
	}
}


void ABotClass::TPExplocation() {
	if (IsValid(TPExplocation_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(TPExplocation_Montage, AnimMontagePlayRate);
	}
}

void ABotClass::LaunchExplotion() {
	TArray<AActor*> OverlappingCharacters;

	FVector ExplotionLocation = ExplotionComponent->GetComponentLocation();
	float ExplotionRadius = ExplotionComponent->GetScaledSphereRadius();

	SphereComponent->ComponentOverlapActors(OverlappingCharacters,
		ACharacterClass::StaticClass(), ExplotionLocation, ExplotionRadius);

	// Iterate over the overlapping actors
	for (AActor* OverlappingCharacter : OverlappingCharacters) {

		// Do something with the overlapping character actor
		if (Player == OverlappingCharacter) {
			/*Deal damage -> to implement*/

		}
	}
}

void ABotClass::CannonAttack() {
	if (IsValid(CannonAttack_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(CannonAttack_Montage, AnimMontagePlayRate);
	}
}

void ABotClass::LaunchCannon() {
	if (IsValid(CannonClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransRightHand = GetMesh()->
			GetSocketTransform(RHSocket, ERelativeTransformSpace::RTS_World);

		AProjectileWeaponClass* Cannon = GetWorld()->SpawnActor<
			AProjectileWeaponClass>(CannonClass, TransRightHand, ActorSpawnParams);
	
		Cannon->SetWeaponController(this);
		Cannon->SetProjectileIgnoreOwner();
		FVector FireDirection = (GetPlayerLocation() - Cannon->GetActorLocation()).GetSafeNormal();
		Cannon->SetProjectileDirection(FireDirection);
	}
}

/* Montage*/
void ABotClass::PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) {
	Super::PlaySelectedAnimMontage(ChosenMontage, PlayRate);

	if (IsValid(Katana)) {
		Katana->SpawnAttachTail();
	}
}

void ABotClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	Super::OnMontageEnded(Montage, bInterrupted);
	
	if (IsValid(Katana)) {
		Katana->Desactivate();
		Katana->DetachDestroyTail();
	}
}

void ABotClass::MoveToEnd() {
	Super::MoveToEnd();

	IsDashing = false;
}