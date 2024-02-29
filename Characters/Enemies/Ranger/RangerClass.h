 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "RainArrowClass.h"

#include "RangerClass.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ARangerClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	ARangerClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	// All Weapon 
		// Single Arrow
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Override MidRange Action")
		void IgnoreProjectile(AProjectileWeaponClass* Projectile);

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void SpawnAttachNormalArrow();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void DetachFireNormalArrow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName ArrowSocket = FName("ArrowSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> NormalArrowClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AProjectileWeaponClass* NormalArrow = nullptr;

		// Knife
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void SpawnAttachKnife();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void DetachFireKnife();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName KnifeSocket = FName("KnifeSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> KnifeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AProjectileWeaponClass* Knife = nullptr;


		// Smoke grenade
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void SpawnAttachGrenade();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void DetachFireGrenade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName GrenadeSocket = FName("GrenadeSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> GrenadeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AProjectileWeaponClass* Grenade = nullptr;

		// Explosive arrow
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void SpawnAttachExplosiveArrow();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void DetachFireExplosiveArrow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName ExplosiveArrowSocket = FName("ExplosiveArrowSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AProjectileWeaponClass> ExplosiveArrowClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AProjectileWeaponClass* ExplosiveArrow = nullptr;

		// Rain Arrow
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void SpawnRainArrow();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class ARainArrowClass> RainArrowClass;


	/* Speed Control*/
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void UpdateFastSpeed(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float FastSpeedValue = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Control Stats")
		float FastSpeedDuration = 0.2f;

	/* Override Actions -> Called in BT*/
	virtual void SelectActionToDo() override;


	/* Unseen Reposition*/


	/* Melee*/
		// Explosive Back
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void BackFlip();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* BackFlip_Montage;

		// Throw Smoke grenade
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void ThrowSmoke();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* ThrowSmoke_Montage;

	/* Mid RANGE*/
		// Single Shot
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SingleShot();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* SingleShot_Montage;

		// Left Leap
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void LeftLeap();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* LeftLeap_Montage;

		// Right Leap
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void RightLeap();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* RightLeap_Montage;

		// Dash Throw Knife
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void DashThrowKnife();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* DashThrowKnife_Montage;

	/* Range*/
		// Confuse Arrow
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void ShotConfuseArrow();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* ShotConfuseArrow_Montage;

		// Leap Forward
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void LeapForward();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* LeapForward_Montage;

		// Leap Forward Extra
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void LeapForwardThrowKnife();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* LeapForwardThrowKnife_Montage;

		// Rain Arrow
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void ShotRainArrow();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* ShotRainArrow_Montage;

public:
	virtual void UnseenRepositionAction() override;
	virtual void MeleeAction() override;
	virtual void MidRangeAction() override;
	virtual void RangedAction() override;

	virtual void PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate);
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void MoveToEnd() override;
};

