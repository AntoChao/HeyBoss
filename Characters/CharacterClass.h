// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// enum
#include "../World/Common_Enums.h"

// Timer
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

// Components
#include "../Components/HealthComponent.h"
#include "../Components/PowerComponent.h"

// sound
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// niagara
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

#include "CharacterClass.generated.h"


UCLASS(BlueprintType, Blueprintable)
class ACharacterClass : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterClass();
	
protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	/* Body Control*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Body Stats")
		float BodyRadius = 42.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Body Stats")
		float BodyHalfHeight = 96.0f;

	/* CHARACTER COMPONENTS*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void InitializeAllCharacterStats();

	/* Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		class UHealthComponent* CharacterHealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Stats")
		float CharacterInitHealth = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetInitCharacterHealth();

	// Take Damage
	
		// Instigator
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, 
		AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual bool IsInstigatorValid(AController* EventInstigator);

		// Get Hit Reaction
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual void TakeDamageReaction();
	
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		void PlayHitReactionMontage();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		UAnimMontage* AnimMontage_HitReaction;

		// Spawn Hit Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage")
		FVector HitEffectLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage")
		FRotator HitEffectRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		class UNiagaraSystem* HitEffectNiagara;
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		void SpawnHitEffectNiagara();

		// Die
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual void ActorDie();

	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual void DieReaction(); /*Animation montage*/
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual void UnpossessController();
	
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual void PlayDieMontage();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		UAnimMontage* AnimMontage_Die;
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual void OnDieMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	/* Power */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
		class UPowerComponent* CharacterPowerComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Stats")
		float CharacterInitPower = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Stats")
		float CharacterInitCritChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Stats")
		float CharacterInitCritMult = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Power")
		void SetInitCharacterPower();


	/* Non - Essencial Movement Stats*/ 
	UFUNCTION(BlueprintCallable, Category = "Power")
		void SetInitCharacterMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float RotationRate = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float JumpZVelocity = 700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float AirControl = 0.35f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float MinWalkSpeed = 20.0f;

	/* Speed factors->walking + running(only main) + boost*/ 
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void ChangeCurrentSpeed();

	// move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float DefaultMoveSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float CurrentMoveSpeed = 500.0f;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void ResetCurrentMoveSpeed();
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void UpdateMoveSpeedRowValue(float value);
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void UpdateMoveSpeedByPercentage(float percentage);

	// run speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float DefaultRunSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float CurrentRunSpeed = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void ResetCurrentRunSpeed();
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void UpdateRunSpeedRowValue(float value);
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void UpdateRunSpeedByPercentage(float percentage);

	// speed boost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FTimerHandle SpeedBoostTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float DefaultSpeedBoost = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Stats")
		float CurrentSpeedBoost = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void ResetCurrentSpeedBoost();
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void UpdateSpeedBoostRowValue(float value, float duration);
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void UpdateSpeedBoostByPercentage(float percentage, float duration);


	/* Behaviour Control*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour Control")
		bool bIsAttacking = false; /* In order to check to do damage or not*/

	/* Custom MoveTo*/
	FTimeline CustomMoveTimeline;
	FComponentQueryParams DefaultComponentQueryParams;

	FCollisionResponseParams DefaultResponseParam;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera View")
		TArray<AActor*> ActorsToIgnore;

	UFUNCTION(BlueprintCallable, Category = "CustomMove Control")
		virtual void InitializeCustomMoveStats();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMove Control")
		bool IsCustomMoveing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMove Control Stats")
		FVector CustomMoveInitLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMove Control Stats")
		FVector CustomMoveTargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMove Control Stats")
		class UCurveFloat* CustomMoveCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMove Control")
		FHitResult CustomMoveCollideHit;

	UFUNCTION(BlueprintCallable, Category = "CustomMove Controller")
		virtual void CustomMove(float MoveDuration, FVector TargetLocation);
	UFUNCTION(BlueprintCallable, Category = "CustomMove Controller")
		virtual void UpdateCustomMoveLocation(float Value);
	UFUNCTION(BlueprintCallable, Category = "CustomMove Controller")
		virtual void CustomMoveEnd();
	UFUNCTION(BlueprintCallable, Category = "CustomMove Controller")
		virtual void StopCustomMove();

	// Animation montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Control Stats")
		float AnimMontagePlayRate = 1.0f;
	UFUNCTION(BlueprintCallable, Category = "Body Control")
		virtual void PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate);
	UFUNCTION(BlueprintCallable, Category = "Body Control")
		virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public: 
	UFUNCTION(BlueprintCallable, Category = "Power")
		UPowerComponent* GetCharacterPower();

	/* Take Damage*/
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		virtual bool IsAbleToTakeDamage();

	// Attaking
	UFUNCTION(BlueprintCallable, Category = "Behaviour Control")
		bool GetIsAttaking();

	// Get Hit Effect
	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		void SetHitEffectStats(FVector Location, FRotator Rotation);
};

