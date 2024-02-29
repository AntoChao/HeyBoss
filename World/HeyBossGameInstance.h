// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Common_Enums.h"
#include "HeyBossGameInstance.generated.h"

UCLASS(minimalapi)
class UHeyBossGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHeyBossGameInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
		EGamePlayState CurrentGameState = EGamePlayState::EPlaying;

public:
	UFUNCTION(BlueprintCallable, Category = "Game State")
		EGamePlayState GetCurrentGameState();

	UFUNCTION(BlueprintCallable, Category = "Game State")
		void SetCurrentGameState(EGamePlayState AState);
};



