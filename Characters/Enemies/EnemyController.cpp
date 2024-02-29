// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyController.h"

// classes
#include "../Player/PlayerClass.h"
#include "EnemyClass.h"

// behavior tree
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// perception
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"

// gameplay
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

/*
GetBlackboardComponent()
		->SetValueAsBool(FName("Main Too Close"), bClose);
GetBlackboardComponent()
			->SetValueAsObject(FName("Charm Target"), CharmActor);
*/

AEnemyController::AEnemyController() {
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void AEnemyController::BeginPlay() {
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddUniqueDynamic(this, &AEnemyController::OnSeePawn);

	// AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SenseMain = false;

	RunBehaviorTree(BehaviorTree);
}

/* Stimulus*/
void AEnemyController::OnSeePawn(APawn* PlayerPawn) {
	APlayerClass* Player = Cast<APlayerClass>(PlayerPawn);

	if (IsValid(Player)) {
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
	else {
		SetCanSeePlayer(false, Player);
	}
}

void AEnemyController::SetCanSeePlayer(bool SeePlayer, UObject* Player) {
	GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), SeePlayer);

	if (SeePlayer) {
		GetBlackboardComponent()->SetValueAsObject(FName("PlayerTarget"), Player);
	}
}

void AEnemyController::RunRetriggerableTimer() {
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle,
		FunctionDelegate, PawnSensing->SensingInterval * 2.0f, false);
}

bool AEnemyController::GetIsSensingPlayer() {
	return SenseMain;
}

/* Finish Intro*/
void AEnemyController::SetIntroFinish(bool isFinished) {
	GetBlackboardComponent()->SetValueAsBool(FName("IntroMontageFinished"), isFinished);
}

void AEnemyController::SetCurrentAction(EEnemyAction NewAction) {
	GetBlackboardComponent()->SetValueAsEnum(FName("CurrentAction"), uint8(NewAction));
}

void AEnemyController::SetRegettingAction(bool CurrentDesicion) {
	GetBlackboardComponent()->SetValueAsBool(FName("RegretAction"), CurrentDesicion);
}

// Positions
void AEnemyController::SetPreAttackPosition(FVector NewLocation) {
	GetBlackboardComponent()->SetValueAsVector(FName("PreAttackPosition"), NewLocation);
}

void AEnemyController::SetActionRequireAttackOffset(bool require) {
	GetBlackboardComponent()->SetValueAsBool(FName("RequireAttackOffset"), require);
}
void AEnemyController::SetAttackOffset(FVector NewLocation) {
	GetBlackboardComponent()->SetValueAsVector(FName("AttackOffset"), NewLocation);
}

void AEnemyController::SetRequireExtraMoveOne(bool require) {
	GetBlackboardComponent()->SetValueAsBool(FName("RequireExtraMoveOne"), require);
}

void AEnemyController::SetRequireExtraMoveTwo(bool require) {
	GetBlackboardComponent()->SetValueAsBool(FName("RequireExtraMoveTwo"), require);
}