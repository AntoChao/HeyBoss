// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToEnd.generated.h"

UCLASS()
class UBTTask_MoveToEnd : public UBTTaskNode
{
	GENERATED_BODY()

private:
	class AEnemyController* OwnerController;

	class AEnemyClass* Owner;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};