// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeyBossGameMode.h"

// player
#include "../Characters/Player/PlayerClass.h"
#include "../Characters/Player/CustomPlayerController.h"

// enemy
#include "../Characters/Enemies/EnemyClass.h"

#include "UObject/ConstructorHelpers.h"

AHeyBossGameMode::AHeyBossGameMode() {

}

void AHeyBossGameMode::BeginPlay() {
	Super::BeginPlay();

	// Game Instance
	GameInstance = Cast<UHeyBossGameInstance>(UGameplayStatics::GetGameInstance(this));

    // Spawn Player
    SpawnPlayer();

	// Sound
	// PlayBGM();
}

/* Level Control*/
void AHeyBossGameMode::OpenPreGameLevel() {
    if (IsValid(GameInstance)) {
        GameInstance->SetCurrentGameState(EGamePlayState::EPreparing);
        UGameplayStatics::OpenLevel(this, FName("PreGameLevel"));
    }
}
void AHeyBossGameMode::OpenInGameLevel() {
    if (IsValid(GameInstance)) {
        GameInstance->SetCurrentGameState(EGamePlayState::EPlaying);
        UGameplayStatics::OpenLevel(this, FName("InGameLevel"));
    }
}
void AHeyBossGameMode::OpenPostGameLevel() {
    if (IsValid(GameInstance)) {
        // Open the level
        GameInstance->SetCurrentGameState(EGamePlayState::EGameOver);
        UGameplayStatics::OpenLevel(this, FName("PostGameLevel"));
    }
}

/* World Control*/

    // World Time
void AHeyBossGameMode::RunRoundTimer() {

    if (TotalSegs == 0 || TotalSegs % RoundDuraction == 0) {
        RoundStarted();
    }

    CurrentSeg++;
    if (CurrentSeg % 60 == 0) {
        CurrentMin++;
        CurrentSeg = 0;
    }

    TotalSegs++;

    GetWorldTimerManager().SetTimer(RoundTimer, this,
        &AHeyBossGameMode::RunRoundTimer, 1, false);
}

void AHeyBossGameMode::RoundStarted() {
    RoundNum++;
    CurrentMin = 0;
    CurrentSeg = 0;

    // More effect like spawning the next enemies
}

    // Player
void AHeyBossGameMode::SpawnPlayer() {
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = 
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (IsValid(PlayerClass)) {
        Player = GetWorld()->SpawnActor<APlayerClass>(PlayerClass, PlayerSpawnLocation,
            PlayerSpawnRotation, ActorSpawnParams);

        // is it the only way, can I spawn a player controller instead of getting it ??
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        if (IsValid(PlayerController) && IsValid(Player)) {
            PlayerController->Possess(Player);
            Player->InitializeController();
        }
    }
}


    // Sound Control
void AHeyBossGameMode::PlayBGM() {
    return;
}

void AHeyBossGameMode::PlayChillBGM() {
    if (IsValid(ChillBGMSoundCue)) {
        BGMComponent = UGameplayStatics::SpawnSound2D(GetWorld(),
            ChillBGMSoundCue);

        if (IsValid(BGMComponent)) {
            BGMComponent->OnAudioFinished.AddUniqueDynamic(this, &AHeyBossGameMode::PlayBGM);
        }
    }
}

void AHeyBossGameMode::PlayCombatBGM() {
    if (IsValid(CombatBGMSoundCue)) {
        BGMComponent = UGameplayStatics::SpawnSound2D(GetWorld(),
            CombatBGMSoundCue);

        if (IsValid(BGMComponent)) {
            BGMComponent->OnAudioFinished.AddUniqueDynamic(this, &AHeyBossGameMode::PlayBGM);
        }
    }
}