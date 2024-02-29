// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../World/Common_Enums.h"

#include "PlayerWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UPlayerWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	// Variables needed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Time")
		class APlayerClass* OnwerPlayer;

	// Variables to display
	/* Time*/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Time")
		int CurrentSeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Time")
		int CurrentHour;

	/* Plyaer Statas*/
		// Player Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		int PlyaerHealth = 0;

		// Player Cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		float PlayerDashCooldown = 0;

		// Mira
	// point -> rifle
	// recharge -> reloading
	// nothing -> blade

		// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		EPrimaryWeapon SelectedWeapon = EPrimaryWeapon::EBlade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		int Ammo = 0;

	/* Enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		TArray<FText> BossesName = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		int BossHealth = 0;

	/* Interaction Dialogue*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Stats")
		bool IsInteracting = false;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Display")
		void InitializeWidget(APlayerClass* APlayer);

	UFUNCTION(BlueprintCallable, Category = "Display Time")
		void SetSeconds(float Seconds);
	UFUNCTION(BlueprintCallable, Category = "Display Time")
		void SetHours(float Hours);

	UFUNCTION(BlueprintCallable, Category = "Display")
		void SetPlayerHealth(float HealthPercentage);

	// Scene -> the talk topic, dialogue -> the phrase
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Display Stats")
		void StartDialogue(AEnemyClass* Enemy, int SceneNumber, int DialogueNumber);

	UFUNCTION(BlueprintCallable, Category = "Display")
		void FinishDialogue();
};

USTRUCT(BlueprintType, Blueprintable)
struct FBossStruct {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;
};


// #include "FBossStruct.generated.h" // Include the generated file