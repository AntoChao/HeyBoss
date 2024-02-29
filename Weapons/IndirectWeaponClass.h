// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponClass.h"

#include "IndirectWeaponClass.generated.h"

/* Indirect weapon are those meshes that spawn direct weapon, such as an gun */

UCLASS()
class AIndirectWeaponClass : public AWeaponClass
{
	GENERATED_BODY()

public:
	AIndirectWeaponClass();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Components*/
	UFUNCTION(BlueprintCallable, Category = "Fire Scene")
		virtual void WeaponFire(FVector TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Stats")
		TSubclassOf<class AProjectileWeaponClass> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Stats")
		int MaxAmmo = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
		int CurrentAmmo = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Fire Stats")
		void SpawnBullet(FVector TargetLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Fire Scene")
		void WeaponFireExtraAction();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Scene Stats")
		USoundCue* WeaponFireSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Scene Stats")
		class UNiagaraSystem* WeaponFireNiagara;

public:
	virtual void Activate(FVector TargetLocation) override;
	virtual void Desactivate() override;

	virtual bool IsAbleToActivate() override;
	virtual void ReloadAmmo() override;

	virtual int GetCurrentAmmo() override;
	virtual void SetCurrentAmmo(int amount) override;
};



