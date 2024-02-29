 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "Components/BoxComponent.h"

#include "AlienUFOClass.generated.h"


UCLASS(BlueprintType, Blueprintable)
class AAlienUFOClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	AAlienUFOClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Component")
		class UBoxComponent* SuckCollisionComp;

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void IntroEndEffect(UAnimMontage* Montage, bool bInterrupted) override;
	
	// Niagara suck
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Suck Control")
		void SpawnSuckNiagara();
	
	// Body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suck Control")
		bool IsAbleToSuck = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suck Control")
		bool IsSucking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suck Control")
		FTimerHandle SuckTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suck Control Stats")
		float SuckCooldown = 5.0f;

	virtual void SelectActionToDo() override;

	UFUNCTION(BlueprintCallable, Category = "Suck Control")
		void SuckFinishedEffect();

	virtual void LifeSpanExpired() override;
};

