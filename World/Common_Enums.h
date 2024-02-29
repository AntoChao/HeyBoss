// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common_Enums.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EGamePlayState : uint8 {
	EPreparing,
	EPlaying,
	EGameOver
};

UENUM(BlueprintType, Blueprintable)
enum class ELanguageSelected : uint8 {
	English,
	Chinese,
	Spanish
};

UENUM(BlueprintType, Blueprintable)
enum class EFloorType : uint8 {
	EConcrete,
	EGrass,
	EDirt,
	ESand
};

UENUM(BlueprintType, Blueprintable)
enum class EPrimaryWeapon : uint8 {
	EBlade,
	EPistol,
	EShotgun,
	ERifle
};


UENUM(BlueprintType, Blueprintable)
enum class EEnemyAction : uint8 {
	EReposition,
	EMeleeAction,
	EMidRangeAction,
	ERangedAction
};


