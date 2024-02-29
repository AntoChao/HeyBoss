// Copyright Epic Games, Inc. All Rights Reserved.

#include "RainArrowClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/BoxComponent.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


ARainArrowClass::ARainArrowClass() {
	SuckCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Coliision Comp"));
	SuckCollisionComp->BodyInstance.SetCollisionProfileName("SuckComp");
	
	//RootComponent = SuckCollisionComp;
	SuckCollisionComp->SetupAttachment(GetMesh());
}

void ARainArrowClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

}

void ARainArrowClass::IntroEndEffect(UAnimMontage* Montage, bool bInterrupted) {
	Super::IntroEndEffect(Montage, bInterrupted);

	IsAbleToSuck = true;

	SpawnSuckNiagara();
}


void ARainArrowClass::SelectActionToDo() {
	Super::SelectActionToDo();
	WrapperMoveToAttackOffset(GetRandomLocation(GetActorLocation(), 10000.0f));
}

void ARainArrowClass::SuckFinishedEffect() {
	if (IsValid(Player)) {
		Player->ActorDie();
	}
}

void ARainArrowClass::LifeSpanExpired() {
	DieReaction();
}