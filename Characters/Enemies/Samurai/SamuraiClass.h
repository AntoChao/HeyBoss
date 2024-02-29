 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "SamuraiClass.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ASamuraiClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	ASamuraiClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	// Weapon -> Body
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void AttachBodyWeapons();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName RHSocket = FName("RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMeleeWeaponClass> KatanaClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* Katana = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ActivateWeapon();
	
	/* Speed Control*/
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void Blink(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float BlinkSpeedValue = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float BlinkSpeedDuration = 0.2f;

	/* Override Actions -> Called in BT*/
	virtual void SelectActionToDo() override;

	/* Unseen Reposition*/
		// just move close to player location randomly
	
	/* Melee*/
		// Get close to do slash attachs randomly
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SingleSlash();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* SingleSlash_Montage;

		// Get close to do a jump slash dash attack
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void RunSlash();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* RunSlash_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		FVector RunSlashVec = FVector(0.0f, 0.0f, 0.0f);

		// Aument the blade to do an AOE
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void AumentKatana();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* AumentKatana_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void AumentKatanaDealDamage();

	/* Mid RANGE*/
		// Charge Wave attack
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void ChargeWave();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		UAnimMontage* ChargeWave_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void SpawnSlashWave();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		TSubclassOf<class AProjectileWeaponClass> SlashWaveClass;


	// Charge fast move front player attack
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void ChargeDash();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		UAnimMontage* ChargeDash_Montage;

		// Double Charge fast move front player attack
		// Polimorphis with before, always using 2 chargedash ???
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void LargeChargeDash();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		UAnimMontage* LargeChargeDash_Montage;

	/* Range*/
		// Spawn floating blades
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void FlotingKatanas();
	
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void PrepareKatanas();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		UAnimMontage* FloatingKatanas_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade1Socket = FName("Blade1Socket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade2Socket = FName("Blade2Socket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade3Socket = FName("Blade3Socket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade4Socket = FName("Blade4Socket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade5Socket = FName("Blade5Socket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade6Socket = FName("Blade6Socket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName Blade7Socket = FName("Blade7Socket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> FloatingBladeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		float MinDelay = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		float MaxDelay = 5.0f;

	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void SpawnFloatingKatana(FName SocketName);

		// Stand Still do lots of attachs which invoke wave slashes
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void InvokeWaves();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		UAnimMontage* InvokeWaves_Montage;

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
};

