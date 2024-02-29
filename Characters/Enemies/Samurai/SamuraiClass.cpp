// Copyright Epic Games, Inc. All Rights Reserved.

#include "SamuraiClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


ASamuraiClass::ASamuraiClass() {
	
}

void ASamuraiClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	AttachBodyWeapons();
}

void ASamuraiClass::AttachBodyWeapons() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(KatanaClass)) {
		FTransform TransRightHand = GetMesh()->
			GetSocketTransform(RHSocket, ERelativeTransformSpace::RTS_World);

		Katana = GetWorld()->SpawnActor<AMeleeWeaponClass>(KatanaClass,
			TransRightHand, ActorSpawnParams);

		if (IsValid(Katana)) {
			Katana->SetWeaponController(this);
			Katana->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, RHSocket);

			ActorsToIgnore.Add(Katana);
		}

		DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
	}
}

void ASamuraiClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bLookingAtPlayer) {
		LookAtPlayer();
	}
}

void ASamuraiClass::ActivateWeapon() {
	if (IsValid(Katana)) {
		Katana->Activate(FVector::ZeroVector);
	}
}

void ASamuraiClass::Blink(FVector Location) {
	SetActorLocation(Location);
	/*
	UpdateSpeedBoostRowValue(BlinkSpeedValue, BlinkSpeedDuration);
	WrapperMoveToAttackOffset(Location);*/
}

	// Override Actions
void ASamuraiClass::SelectActionToDo() {
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
void ASamuraiClass::UnseenRepositionAction() {
	Super::UnseenRepositionAction();
	
	// Get Close to Player
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Move Close To Player"));
	
	FVector Direction = GetPlayerSameHeightDirection();
	float Distance = GetPlayerDistance();

	// Check Samurai back
	
	float MinDistance = FMath::Min(RangedRange, Distance);

	float Offset = FMath::RandRange(10.0f, MinDistance);

	FVector Destination = GetActorLocation() + Direction * (Distance - Offset);

	WrapperMoveToAttackOffset(Destination);
}

// Melee
void ASamuraiClass::MeleeAction() {
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

	// Get close to do slash attachs randomly
void ASamuraiClass::SingleSlash() {
	if (IsValid(Katana)) {
		if (IsValid(SingleSlash_Montage)) {
			PlaySelectedAnimMontage(SingleSlash_Montage, AnimMontagePlayRate);
		}
	}
}
	
	// Get close to do a jump slash dash attack
void ASamuraiClass::RunSlash() {
	if (IsValid(Katana)) {
		if (IsValid(RunSlash_Montage)) {

			PlaySelectedAnimMontage(RunSlash_Montage, AnimMontagePlayRate);
			WrapperMoveToAttackOffset(GetPlayerFrontPosition());
		}
	}
}

	// Aument the blade to do an AOE
void ASamuraiClass::AumentKatana() {
	if (IsValid(Katana)) {
		if (IsValid(AumentKatana_Montage)) {

			PlaySelectedAnimMontage(AumentKatana_Montage, AnimMontagePlayRate);
		}
	}
}

void ASamuraiClass::AumentKatanaDealDamage() {
	return;
}

// Mid Range
void ASamuraiClass::MidRangeAction() {
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

void ASamuraiClass::ChargeWave() {
	if (IsValid(Katana)) {
		if (IsValid(ChargeWave_Montage)) {
			PlaySelectedAnimMontage(ChargeWave_Montage, AnimMontagePlayRate);
		}
	}
}

void ASamuraiClass::SpawnSlashWave() {
	if (IsValid(Katana)) {
		if (IsValid(SlashWaveClass)) {
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform FireTrans = Katana->GetWeaponFireSocketTransform();

			AProjectileWeaponClass* SlashWave = GetWorld()->SpawnActor<AProjectileWeaponClass>(
				SlashWaveClass, FireTrans, ActorSpawnParams);

			if (IsValid(SlashWave)) {
				SlashWave->SetWeaponController(this);
				SlashWave->SetProjectileIgnoreOwner();

				// IgnoreDBBall(DBBall); maybe need this blueprintimplementable

				FVector FireDirection = (GetPlayerLocation() - SlashWave->GetActorLocation()).GetSafeNormal();
				SlashWave->SetProjectileDirection(FireDirection);
			}
		}
	}
}

void ASamuraiClass::ChargeDash() {
	if (IsValid(Katana)) {
		if (IsValid(ChargeDash_Montage)) {
			PlaySelectedAnimMontage(ChargeDash_Montage, AnimMontagePlayRate);
		}
	}
}

void ASamuraiClass::LargeChargeDash() {
	if (IsValid(Katana)) {
		if (IsValid(LargeChargeDash_Montage)) {
			PlaySelectedAnimMontage(LargeChargeDash_Montage, AnimMontagePlayRate);
			
		}
	}
}

// Range Action
void ASamuraiClass::RangedAction() {
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

void ASamuraiClass::FlotingKatanas() {
	if (IsValid(Katana)) {
		if (IsValid(FloatingKatanas_Montage)) {
			PlaySelectedAnimMontage(FloatingKatanas_Montage, AnimMontagePlayRate);
		}
	}
}

// Katanas should be self launched
void ASamuraiClass::PrepareKatanas() {
	if (IsValid(FloatingBladeClass)) {
		SpawnFloatingKatana(Blade1Socket);
		SpawnFloatingKatana(Blade2Socket);
		SpawnFloatingKatana(Blade3Socket);
		SpawnFloatingKatana(Blade4Socket);
		SpawnFloatingKatana(Blade5Socket);
		SpawnFloatingKatana(Blade6Socket);
		SpawnFloatingKatana(Blade7Socket);
	}
}

void ASamuraiClass::SpawnFloatingKatana(FName SocketName) {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform FloatingKatanaTrans;
	FloatingKatanaTrans = GetMesh()->GetSocketTransform(
		SocketName, ERelativeTransformSpace::RTS_World);

	AProjectileWeaponClass* FloatingKatana = GetWorld()->SpawnActor<AProjectileWeaponClass>(
		FloatingBladeClass, FloatingKatanaTrans, ActorSpawnParams);
	if (IsValid(FloatingKatana)) {
		FloatingKatana->SetWeaponController(this);
		if (IsValid(Player)) {
			FloatingKatana->SetWeaponTarget(Player);

			float Delay = FMath::RandRange(MinDelay, MaxDelay);
			FloatingKatana->DelayFlyToTarget(Delay);
		}
	}
	
}


void ASamuraiClass::InvokeWaves() {
	if (IsValid(Katana)) {
		if (IsValid(InvokeWaves_Montage)) {
			PlaySelectedAnimMontage(InvokeWaves_Montage, AnimMontagePlayRate);
		}
	}
}

/* Montage*/
void ASamuraiClass::PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) {
	Super::PlaySelectedAnimMontage(ChosenMontage, PlayRate);

	if (IsValid(Katana)) {
		Katana->SpawnAttachTail();
	}
}

void ASamuraiClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	Super::OnMontageEnded(Montage, bInterrupted);
	
	if (IsValid(Katana)) {
		Katana->Desactivate();
		Katana->DetachDestroyTail();
	}
}