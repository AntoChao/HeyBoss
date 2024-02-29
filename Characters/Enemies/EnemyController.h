// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "TimerManager.h"

#include "../../World/Common_Enums.h"

#include "EnemyController.generated.h"


UCLASS(BlueprintType, Blueprintable)
class AEnemyController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyController();

protected:	
	// To add mapping context
	virtual void BeginPlay();

	/* Behaviour Tree*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour Tree")
		class UBehaviorTree* BehaviorTree;

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "Behaviour Tree")
		void RunRetriggerableTimer();

	/* Enemy Sensing/ Stimulus*/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stimulus Control")
		class UPawnSensingComponent* PawnSensing;

	UFUNCTION(BlueprintCallable, Category = "Stimulus Control")
		void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = "Stimulus Control")
		void SetCanSeePlayer(bool SeePlayer, class UObject* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stimulus Control")
		bool SenseMain;

	
public:
	/* Enemy Sensing/ Stimulus*/
	UFUNCTION(BlueprintCallable, Category = "Stimulus Control")
		bool GetIsSensingPlayer();

	/* Finish Intro*/
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetIntroFinish(bool isFinished);

	/* Enemy Actions*/
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetCurrentAction(EEnemyAction NewAction);
		
	// Regret the action 
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetRegettingAction(bool CurrentDesicion);

	// Attack positions
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetPreAttackPosition(FVector NewLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetActionRequireAttackOffset(bool require);
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetAttackOffset(FVector NewLocation);

	// Extra movement
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetRequireExtraMoveOne(bool require);
	UFUNCTION(BlueprintCallable, Category = "Action Control")
		void SetRequireExtraMoveTwo(bool require);

};

