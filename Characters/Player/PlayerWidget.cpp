// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerClass.h"
#include "Math/UnrealMathUtility.h"

void UPlayerWidget::InitializeWidget(APlayerClass* APlayer) {
	OnwerPlayer = APlayer;
}

// Character Level
void UPlayerWidget::SetPlayerHealth(float HealthPercentage) {
	PlyaerHealth = HealthPercentage;
}

// Time
void UPlayerWidget::SetSeconds(float Seconds) {
	CurrentSeg = FMath::RoundToInt(Seconds);
}
void UPlayerWidget::SetHours(float Hours) {
	CurrentHour = FMath::RoundToInt(Hours);
}

void UPlayerWidget::FinishDialogue() {
	if (IsValid(OnwerPlayer)) {
		IsInteracting = false;
		OnwerPlayer->DisableInteraction();
	}
}