// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeyBossGameInstance.h"

#include "UObject/ConstructorHelpers.h"

UHeyBossGameInstance::UHeyBossGameInstance(){

}

EGamePlayState UHeyBossGameInstance::GetCurrentGameState() {
	return CurrentGameState;
}

void UHeyBossGameInstance::SetCurrentGameState(EGamePlayState AState) {
	CurrentGameState = AState;
}