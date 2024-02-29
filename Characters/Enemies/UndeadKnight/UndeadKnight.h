 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "BloodCloud.h"
#include "Components/SphereComponent.h"
#include "UndeadKnight.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AUndeadKnight : public AEnemyClass
{
	GENERATED_BODY()

public:
	AUndeadKnight();

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	// Weapon -> Body
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void AttachBodyWeapons();

	// left hand
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName LHSocket = FName("LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMeleeWeaponClass> KnightSwordClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* KnightSword = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ActivateWeapon();

	// Blood Clouds control	
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void SpawnBloodCloud();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TArray<ABloodCloud*> SpawnedBloodClouds = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class ABloodCloud> BloodCloudClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName BloodCloudSocket = FName("BloodCloudSocket");

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ExplodeAllBloodCloud();

	/* Override Actions -> Called in BT*/
	virtual void SelectActionToDo() override;

	/* Unseen Reposition*/


	/* Melee*/
		// Single Melee Attack
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SingleMeleeAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* SingleMeleeAttack_Montage;

	/* Mid RANGE*/
		// Charge
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void StartCharge();
	
		// Speed Control
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void UpdateChargeSpeed(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		bool IsCharging = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float ChargeSpeedValue = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float ChargeSpeedDuration = 0.2f;

		// Ground Wave
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void HitGround();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* HitGround_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SpawnGroundWave();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> WaveClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName GroundWaveSocket = FName("GroundWaveSocket");

		// Died Solder Wall
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SolderWall();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* SolderWall_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SpawnSolderWall();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> SolderWallClass;

	/* Range*/
		
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
};

