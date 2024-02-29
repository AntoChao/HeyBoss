 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"

#include "AlienBloomClass.generated.h"


UCLASS(BlueprintType, Blueprintable)
class AAlienBloomClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	AAlienBloomClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

};

