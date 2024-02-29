// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FollowActionOne.generated.h"

UCLASS()
class UBTTask_FollowActionOne : public UBTTaskNode
{
	GENERATED_BODY()

private:
	class AEnemyController* OwnerController;

	class AEnemyClass* Owner;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds);

};