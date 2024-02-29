// Copyright Epic Games, Inc. All Rights Reserved.

#include "UndeadKnight.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


AUndeadKnight::AUndeadKnight() {
	ExplotionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Explotion Range"));
	
	ExplotionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	ExplotionComponent->InitSphereRadius(50.0f);
}

void AUndeadKnight::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	AttachBodyWeapons();
}

void AUndeadKnight::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsCharging) {
		WrapperMoveToAttackOffset(GetPlayerLocation());
	}
}

void AUndeadKnight::AttachBodyWeapons() {
	if (IsValid(KnightSwordClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransLeftHand = GetMesh()->
			GetSocketTransform(LHSocket, ERelativeTransformSpace::RTS_World);

		KnightSword = GetWorld()->SpawnActor<AMeleeWeaponClass>(KnightSwordClass,
			TransLeftHand, ActorSpawnParams);

		if (IsValid(KnightSword)) {
			KnightSword->SetWeaponController(this);
			KnightSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, LHSocket);

			ActorsToIgnore.Add(KnightSword);
		}

		DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
	}
}

void AUndeadKnight::ActivateWeapon() {
	if (IsValid(KnightSword)) {
		KnightSword->Activate(FVector::ZeroVector);
	}
}

void AUndeadKnight::SpawnBloodCloud() {
	if (IsValid(BloodCloudClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransBloodCloud = GetMesh()->
			GetSocketTransform(BloodCloudSocket, ERelativeTransformSpace::RTS_World);

		ABloodCloud* BCDelta = GetWorld()->SpawnActor<ABloodCloud>(BloodCloudClass,
			TransBloodCloud, ActorSpawnParams);
		SpawnedBloodClouds.Add(BCDelta);
	}
}

void AUndeadKnight::ExplodeAllBloodCloud() {
	for (ABloodCloud* SpawnedBloodCloud : SpawnedBloodClouds) {
		if (IsValid(SpawnedBloodCloud)) {
			SpawnedBloodCloud->Explode();
		}
	}
	SpawnedBloodClouds = {};
}

	// Override Actions
void AUndeadKnight::SelectActionToDo() {
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
void AUndeadKnight::UnseenRepositionAction() {
	Super::UnseenRepositionAction();
	
	// Get Close to Player
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Move Close To Player"));
	
}

// Melee
void AUndeadKnight::MeleeAction() {
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
void AUndeadKnight::SingleMeleeAttack() {
	if (IsValid(SingleMeleeAttack_Montage)) {
		PlaySelectedAnimMontage(SingleMeleeAttack_Montage, AnimMontagePlayRate);
	}
}

// Mid Range
void AUndeadKnight::MidRangeAction() {
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

// Charge
void AUndeadKnight::StartCharge() {
	IsCharging = true;
	UpdateChargeSpeed(FVector::Zero);
}

void AUndeadKnight::UpdateChargeSpeed(FVector Location) {
	UpdateSpeedBoostRowValue(ChargeSpeedValue, ChargeSpeedDuration);
	// WrapperMoveToAttackOffset(Location);
}


// Ground Wave
void AUndeadKnight::HitGround() {
	if (IsValid(HitGround_Montage)) {
		PlaySelectedAnimMontage(HitGround_Montage, AnimMontagePlayRate);
	}
}

void AUndeadKnight::SpawnGroundWave() {
	if (IsValid(WaveClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransGroundWave = GetMesh()->
			GetSocketTransform(GroundWaveSocket, ERelativeTransformSpace::RTS_World);

		AProjectileWeaponClass* AGroundWave = GetWorld()->SpawnActor<AProjectileWeaponClass>(WaveClass,
			TransGroundWave, ActorSpawnParams);

		if (IsValid(AGroundWave)) {
			AGroundWave->SetWeaponController(this);

			IgnoreProjectile(AGroundWave); /* implement in blueprint*/

			ActorsToIgnore.Add(AGroundWave);
			DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
		}
	}
}

// Died Solder Wall
void AUndeadKnight::SolderWall() {
	if (IsValid(SolderWall_Montage)) {
		PlaySelectedAnimMontage(SolderWall_Montage, AnimMontagePlayRate);
	}
}

void AUndeadKnight::SpawnSolderWall() {
	if (IsValid(SolderWallClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		
		AProjectileWeaponClass* AGroundWave = GetWorld()->SpawnActor<AProjectileWeaponClass>(SolderWallClass,
			TransGroundWave, ActorSpawnParams);

		if (IsValid(AGroundWave)) {
			AGroundWave->SetWeaponController(this);

			IgnoreProjectile(AGroundWave); /* implement in blueprint*/

			ActorsToIgnore.Add(AGroundWave);
			DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
		}
	}
}

// Range Action
void AUndeadKnight::RangedAction() {
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

/* Montage*/
void AUndeadKnight::PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) {
	Super::PlaySelectedAnimMontage(ChosenMontage, PlayRate);

	if (IsValid(Katana)) {
		Katana->SpawnAttachTail();
	}
}

void AUndeadKnight::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	Super::OnMontageEnded(Montage, bInterrupted);
	
	if (IsValid(KnightSword)) {
		KnightSword->Desactivate();
		KnightSword->DetachDestroyTail();
	}
}

void AUndeadKnight::MoveToEnd() {
	Super::MoveToEnd();

	IsCharging = false;
}