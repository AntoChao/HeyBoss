 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"
#include "MiniBotClass.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AMiniBotClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	AMiniBotClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

};

