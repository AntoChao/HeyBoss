// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponClass.h"

#include "DirectWeaponClass.generated.h"

/* Direct weapon are those which deal damage, like blade, bullet 
	They have collision/ hit result and check to do damage
*/

UCLASS()
class ADirectWeaponClass : public AWeaponClass
{
	GENERATED_BODY()

public:
	ADirectWeaponClass();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

		// Niagara collision
	UFUNCTION(BlueprintCallable, Category = "OverlapEvent")
		void PlayHitCharacterEffect(FHitResult HitResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapEvent Sound Stats")
		USoundCue* HitEffectCharacterSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapEvent Niagara Stats")
		class UNiagaraSystem* HitEffectCharacterNiagara;

	UFUNCTION(BlueprintCallable, Category = "OverlapEvent")
		void PlayHitEnvironmentEffect(FHitResult HitResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapEvent Sound Stats")
		USoundCue* HitEffectEnvironmentSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapEvent Niagara Stats")
		class UNiagaraSystem* HitEffectEnvironmentNiagara;

	UFUNCTION(BlueprintCallable, Category = "OverlapEvent")
		void PlayHitNewWeaponEffect(FHitResult HitResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapEvent Sound Stats")
		USoundCue* HitNewWeaponSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OverlapEvent Niagara Stats")
		class UNiagaraSystem* HitNewWeaponNiagara;

		// Power Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Component")
		class UPowerComponent* WeaponPowerComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Stats")
		float WeaponInitPower = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Stats")
		float WeaponInitCritChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Stats")
		float WeaponInitCritMult = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Power")
		void SetInitWeaponPower();

		// Damage
	UFUNCTION(BlueprintCallable, Category = "Damage")
		float CalculateGeneralDamage(UPowerComponent* CharacterPowerComponent);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		bool CritictStrike(float Probability);

		// Apply Damage
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void ApplyOverlapDamage(ACharacterClass* ACharacter, FHitResult HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
		bool IsActivated = false;

public:
	virtual void Activate(FVector TargetLocation) override;
	virtual void Desactivate() override;
};



