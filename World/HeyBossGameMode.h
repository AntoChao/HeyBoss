// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// game world 
#include "HeyBossGameInstance.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

#include "HeyBossGameMode.generated.h"

UCLASS(minimalapi)
class AHeyBossGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHeyBossGameMode();

protected:
	virtual void BeginPlay() override;
	
	/* Level Control*/
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		void OpenPreGameLevel();
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		void OpenInGameLevel();
	UFUNCTION(BlueprintCallable, Category = "Level Control")
		void OpenPostGameLevel();

	/* World Control*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Instance")
		class UHeyBossGameInstance* GameInstance;

		// Round Time -> x minutes as break
	UFUNCTION(BlueprintCallable, Category = "Time Control")
		void RunRoundTimer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		FTimerHandle RoundTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		int TotalSegs = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		int RoundNum = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		int CurrentMin = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		int CurrentSeg = 0;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Control")
		int RoundDuraction = 300; /*5 minutes ->  300 segs*/
	UFUNCTION(BlueprintCallable, Category = "Time Control")
		void RoundStarted();


		// Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Control")
		TSubclassOf<class APlayerClass> PlayerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Control")
		class APlayerClass* Player = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Control")
		FVector PlayerSpawnLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Control")
		FRotator PlayerSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	UFUNCTION(BlueprintCallable, Category = "BGM")
		void SpawnPlayer();


		// World sound
	UFUNCTION(BlueprintCallable, Category = "BGM")
		void PlayBGM();

	UFUNCTION(BlueprintCallable, Category = "BGM")
		void PlayChillBGM();
	UFUNCTION(BlueprintCallable, Category = "BGM")
		void PlayCombatBGM();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
		UAudioComponent* BGMComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
		USoundCue* ChillBGMSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
		USoundCue* CombatBGMSoundCue;
};



