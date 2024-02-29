// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniBotClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


AMiniBotClass::AMiniBotClass() {
	
}

void AMiniBotClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	AttachBodyWeapons();
}