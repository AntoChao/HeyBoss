// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlienPortalClass.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "../Characters/CharacterClass.h"

#include "Math/UnrealMathUtility.h"


AAlienPortalClass::AAlienPortalClass(){


}

void AAlienPortalClass::BeginPlay() {
	Super::BeginPlay();
}

void AAlienPortalClass::PlayerInAreaResponse(APlayerClass* player) {
	if (PortalIsActivated) {
		if (IsValid(player)) {
			player->SetActorLocation(TeleportLocation);
		}
	}
}

void AAlienPortalClass::SetTeleport(ACharacterClass* ItsOwner, bool Activation, FVector ATPLocation) {
	PortalIsActivated = Activation;
	PortalOwner = ItsOwner;
	TeleportLocation = ATPLocation;
}