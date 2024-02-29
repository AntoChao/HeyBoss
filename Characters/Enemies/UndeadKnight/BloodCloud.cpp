// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodCloud.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


ABloodCloud::ABloodCloud() {
	
}

void ABloodCloud::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	AttachBodyWeapons();
}

	// Do damage around and destroy it self 
void ABloodCloud::Explode() {
	return;
}