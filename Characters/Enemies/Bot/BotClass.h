 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "MiniBotClass.h"
#include "Components/SphereComponent.h"
#include "BotClass.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ABotClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	ABotClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		USphereComponent* ExplotionComponent = nullptr;

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	// If is a bot clone, the only thing is know is dash and die -> dif ai control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		bool IsBotClone = false;

	// Weapon -> Body
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void AttachBodyWeapons();

	// left hand
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName LHSocket = FName("LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMeleeWeaponClass> LeftHandWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* LeftHandWeapon = nullptr;

	// right hand
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName RHSocket = FName("RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMeleeWeaponClass> RightHandWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* RightHandWeapon = nullptr;

	// hair
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName HairSocket = FName("HairSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMeleeWeaponClass> HairWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* HairWeapon = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ActivateWeapon();

	/* Speed Control*/
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void UpdateDashSpeed(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float DashSpeedValue = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float DashSpeedDuration = 0.2f;

	/* Override Actions -> Called in BT*/
	virtual void SelectActionToDo() override;



	/* Unseen Reposition*/


	/* Melee*/
		// Circle Attack
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void TurnCircleAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* TurnCircleAttack_Montage;

	// TP Away
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void TPAway();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* TPAway_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void PlayTPEffect();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		float TPDistance = 5000.0f;


	/* Mid RANGE*/
		// Dash Attack -> update speed, activate weapon and animation blueprint
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void DashAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		float DashAttackDistance = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		bool IsDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		FHitResult DashCollideHit;

	// Create two bot clons
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void BotCloneAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* BotCloneAttack_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SpawnBotAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action ")
		TSubclassOf<class ABotClass> BotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action ")
		float BotSpawnDistance = 500.0f;


	// BotArena	-> Create 10 clones that dash through from one 
					// side to another side of arena
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void BotArena();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* BotArena_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SpawnBotArena();
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		bool IsNotOkayToSpawnBotArena(FVector SpawnLocation);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller")
		int BotArenaCounts = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller")
		int TrySpawnBotArenaTimes = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller")
		float MinimalSpawnDistnace = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller")
		FTimerHandle BotArenaTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller Stats")
		float BotArenaCooldown = 2.0f;

		// Duplicate -> Literal Bot Clone
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void Duplicate();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* Duplicate_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SpawnDuplicateBot();

	/* Range*/
		// Spawn MiniBot
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SpawnMiniBot();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* SpawnMiniBot_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMiniBotClass> MiniBotClass;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void LaunchMiniBot();

		// TP Explotion
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void TPExplocation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* TPExplocation_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void LaunchExplotion();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		class UNiagaraSystem* ExplotionNiagara;

		// Cannon Attack
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void CannonAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* CannonAttack_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void LaunchCannon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		TSubclassOf<class AProjectileWeaponClass> CannonClass;

public:
	virtual void UnseenRepositionAction() override;
	virtual void MeleeAction() override;
	virtual void MidRangeAction() override;
	virtual void RangedAction() override;
	/*
	virtual void FollowActionOne() override;
	virtual void FollowActionTwo() override;*/

	virtual void PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate);
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void MoveToEnd() override;

	UFUNCTION(BlueprintCallable, Category = "Clone Control")
		void SetIsClone(bool NewAttribute);
};

