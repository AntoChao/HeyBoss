 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "BloodCloud.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ABloodCloud : public AEnemyClass
{
	GENERATED_BODY()

public:
	ABloodCloud();

protected:
	// To add mapping context
	virtual void BeginPlay();

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void Explode();

};

