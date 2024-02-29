// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlienUFOClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/BoxComponent.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


AAlienUFOClass::AAlienUFOClass() {
	SuckCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Coliision Comp"));
	SuckCollisionComp->BodyInstance.SetCollisionProfileName("SuckComp");
	
	//RootComponent = SuckCollisionComp;
	SuckCollisionComp->SetupAttachment(GetMesh());
}

void AAlienUFOClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

}

void AAlienUFOClass::IntroEndEffect(UAnimMontage* Montage, bool bInterrupted) {
	Super::IntroEndEffect(Montage, bInterrupted);

	IsAbleToSuck = true;

	SpawnSuckNiagara();
}


void AAlienUFOClass::SelectActionToDo() {
	Super::SelectActionToDo();
	WrapperMoveToAttackOffset(GetRandomLocation(GetActorLocation(), 10000.0f));
}

void AAlienUFOClass::SuckFinishedEffect() {
	if (IsValid(Player)) {
		Player->ActorDie();
	}
}

void AAlienUFOClass::LifeSpanExpired() {
	DieReaction();
}