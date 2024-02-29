// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTTask_FollowActionTwo.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"

EBTNodeResult::Type UBTTask_FollowActionTwo::
ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	OwnerController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	Owner = Cast<AEnemyClass>(OwnerController->GetPawn());

	bNotifyTick = 1;

	if (!IsValid(OwnerController)) {
		return EBTNodeResult::Failed;
	}
	if (!IsValid(Owner)) {
		return EBTNodeResult::Failed;
	}

	Owner->FollowActionOne();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_FollowActionTwo::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds) {
	
	if (!Owner->GetDoingAnyAction()) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}