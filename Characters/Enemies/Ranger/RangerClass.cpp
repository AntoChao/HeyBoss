// Copyright Epic Games, Inc. All Rights Reserved.

#include "RangerClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


ARangerClass::ARangerClass() {
	ExplotionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Explotion Range"));
	
	ExplotionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	ExplotionComponent->InitSphereRadius(50.0f);
}

void ARangerClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	AttachBodyWeapons();
}

void ARangerClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bLookingAtPlayer) {
		LookAtPlayer();
	}
}

void ARangerClass::SpawnAttachNormalArrow() {
	// left hand
	if (IsValid(NormalArrowClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransArrow = GetMesh()->
			GetSocketTransform(ArrowSocket, ERelativeTransformSpace::RTS_World);

		NormalArrow = GetWorld()->SpawnActor<AProjectileWeaponClass>(NormalArrowClass,
			TransArrow, ActorSpawnParams);

		if (IsValid(NormalArrow)) {
			NormalArrow->SetWeaponController(this);
			NormalArrow->SetProjectileIgnoreOwner();
			
			/* Maybe change the collision*/

			IgnoreProjectile(NormalArrow); /* implement in blueprint*/
			
			NormalArrow->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, ArrowSocket);

			ActorsToIgnore.Add(NormalArrow);
			DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
		}
	}
}

void ARangerClass::DetachFireNormalArrow() {
	if (IsValid(NormalArrow)) {
		NormalArrow->DetachFromActor(FDetachmentTransformRules::
			KeepWorldTransform);

		FVector FireDirection = (GetPlayerLocation() - 
			NormalArrow->GetActorLocation()).GetSafeNormal();
		NormalArrow->SetProjectileDirection(FireDirection);
	}
}

	// Knife
void ARangerClass::SpawnAttachKnife() {
	// left hand
	if (IsValid(KnifeClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransKnife = GetMesh()->
			GetSocketTransform(KnifeSocket, ERelativeTransformSpace::RTS_World);

		Knife = GetWorld()->SpawnActor<AProjectileWeaponClass>(KnifeClass,
			TransKnife, ActorSpawnParams);

		if (IsValid(Knife)) {
			Knife->SetWeaponController(this);
			Knife->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, KnifeSocket);

			IgnoreProjectile(Knife); /* implement in blueprint*/

			ActorsToIgnore.Add(Knife);
			DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
		}
	}
}

void ARangerClass::DetachFireKnife() {
	if (IsValid(Knife)) {
		Knife->DetachFromActor(FDetachmentTransformRules::
			KeepWorldTransform);

		FVector FireDirection = (GetPlayerLocation() -
			Knife->GetActorLocation()).GetSafeNormal();
		Knife->SetProjectileDirection(FireDirection);
	}
}


// Smoke Grenade
void ARangerClass::SpawnAttachGrenade() {
	// left hand
	if (IsValid(GrenadeClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransGrenade = GetMesh()->
			GetSocketTransform(GrenadeSocket, ERelativeTransformSpace::RTS_World);

		Grenade = GetWorld()->SpawnActor<AProjectileWeaponClass>(GrenadeClass,
			TransGrenade, ActorSpawnParams);

		if (IsValid(Grenade)) {
			Grenade->SetWeaponController(this);
			Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, GrenadeSocket);

			IgnoreProjectile(Grenade); /* implement in blueprint*/

			ActorsToIgnore.Add(Grenade);
			DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
		}
	}
}

void ARangerClass::DetachFireGrenade() {
	if (IsValid(Grenade)) {
		Grenade->DetachFromActor(FDetachmentTransformRules::
			KeepWorldTransform);

		FVector FireDirection = (GetPlayerLocation() -
			Grenade->GetActorLocation()).GetSafeNormal();
		Grenade->SetProjectileDirection(FireDirection);
	}
}

// Explosive Arrow
void ARangerClass::SpawnAttachExplosiveArrow() {
	// left hand
	if (IsValid(ExplosiveArrowClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransExplosiveArrow = GetMesh()->
			GetSocketTransform(ExplosiveArrowSocket, ERelativeTransformSpace::RTS_World);

		ExplosiveArrow = GetWorld()->SpawnActor<AProjectileWeaponClass>(ExplosiveArrowClass,
			TransExplosiveArrow, ActorSpawnParams);

		if (IsValid(ExplosiveArrow)) {
			ExplosiveArrow->SetWeaponController(this);
			ExplosiveArrow->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, ExplosiveArrowSocket);

			IgnoreProjectile(ExplosiveArrow); /* implement in blueprint*/

			ActorsToIgnore.Add(ExplosiveArrow);
			DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
		}
	}
}

void ARangerClass::DetachFireExplosiveArrow() {
	if (IsValid(ExplosiveArrow)) {
		ExplosiveArrow->DetachFromActor(FDetachmentTransformRules::
			KeepWorldTransform);

		FVector FireDirection = (GetPlayerLocation() -
			ExplosiveArrow->GetActorLocation()).GetSafeNormal();
		ExplosiveArrow->SetProjectileDirection(FireDirection);
	}
}


// Rain Arrow
void ARangerClass::SpawnRainArrow() {
	if (IsValid(RainArrowClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ARainArrowClass>(RainArrowClass,
			GetPlayerLocation(), GetActorRotation(), ActorSpawnParams);
	}
}


void ARangerClass::UpdateFastSpeed(FVector Location) {
	UpdateSpeedBoostRowValue(FastSpeedValue, FastSpeedDuration);
	WrapperMoveToAttackOffset(Location);
}

	// Override Actions
void ARangerClass::SelectActionToDo() {
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
void ARangerClass::UnseenRepositionAction() {
	Super::UnseenRepositionAction();
	
	// Get Close to Player
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Move Close To Player"));
	
}

// Melee
void ARangerClass::MeleeAction() {
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

// Explosive Back
void ABotClass::BackFlip() {
	if (IsValid(BackFlip_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(BackFlip_Montage, AnimMontagePlayRate);
	}
}

// Throw Smoke grenade
void ABotClass::ThrowSmoke() {
	if (IsValid(ThrowSmoke_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(ThrowSmoke_Montage, AnimMontagePlayRate);
	}
}

// Mid Range
void ARangerClass::MidRangeAction() {
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

// Single Shot
void ABotClass::ThrowSmoke() {
	if (IsValid(ThrowSmoke_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(ThrowSmoke_Montage, AnimMontagePlayRate);
	}
}

// Left Leap
void ABotClass::LeftLeap() {
	if (IsValid(LeftLeap_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(LeftLeap_Montage, AnimMontagePlayRate);
	}
}

// Right Leap
void ABotClass::RightLeap() {
	if (IsValid(RightLeap_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(RightLeap_Montage, AnimMontagePlayRate);
	}
}

// Dash Throw Knife
void ABotClass::DashThrowKnife() {
	if (IsValid(DashThrowKnife_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(DashThrowKnife_Montage, AnimMontagePlayRate);
	}
}

// Range Action
void ARangerClass::RangedAction() {
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

// Confuse Arrow
void ABotClass::DashThrowKnife() {
	if (IsValid(ShotConfuseArrow_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(ShotConfuseArrow_Montage, AnimMontagePlayRate);
	}
}

// Leap Forward
void ABotClass::DashThrowKnife() {
	if (IsValid(LeapForward_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(LeapForward_Montage, AnimMontagePlayRate);
	}
}

// Leap Forward Extra
void ABotClass::DashThrowKnife() {
	if (IsValid(LeapForwardThrowKnife_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(LeapForwardThrowKnife_Montage, AnimMontagePlayRate);
	}
}

// Rain Arrow
void ABotClass::DashThrowKnife() {
	if (IsValid(ShotRainArrow_Montage)) {
		LookAtPlayer();
		PlaySelectedAnimMontage(ShotRainArrow_Montage, AnimMontagePlayRate);
	}
}


/* Montage*/
void ARangerClass::PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) {
	Super::PlaySelectedAnimMontage(ChosenMontage, PlayRate);

	if (IsValid(Katana)) {
		Katana->SpawnAttachTail();
	}
}

void ARangerClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	Super::OnMontageEnded(Montage, bInterrupted);
	
	if (IsValid(Katana)) {
		Katana->Desactivate();
		Katana->DetachDestroyTail();
	}
}

void ARangerClass::MoveToEnd() {
	Super::MoveToEnd();

	IsDashing = false;
}