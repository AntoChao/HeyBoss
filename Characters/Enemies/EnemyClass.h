// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Components
#include "Components/SphereComponent.h"

// Class to Recognize
#include "../CharacterClass.h"
#include "../Player/PlayerClass.h"
#include "../Player/CustomPlayerController.h"
#include "EnemyController.h"

#include "EnemyClass.generated.h"

/*
BLUEPRINTCALLABLE FUNCTION PARAMETER FORM
	DECLARE_DYNAMIC_DELEGATE(FEnemyDelegate);
	DECLARE_DELEGATE(FEnemyDelegate);

	UFUNCTION(Category = "Default Action Control")
		void DoActionWithOffset(int OriginalChance, FEnemyDelegate OriginalAction,
			FEnemyDelegate SecondaryAction);
*/

UCLASS(BlueprintType, Blueprintable)
class AEnemyClass : public ACharacterClass
{
	GENERATED_BODY()

public:
	AEnemyClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	/* Common Properties*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Control")
		class UAIPerceptionStimuliSourceComponent* Stimulus;
	UFUNCTION(BlueprintCallable, Category = "Default Control")
		void SetupStimulus();

	UPROPERTY(EditAnywhere, Category = "Interact Collision")
		class USphereComponent* InteractCollisionComp;

	UFUNCTION(BlueprintCallable, Category = "Default Control")
		void InitializeProperties();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Control")
		class AEnemyController* EnemyController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Control")
		class APlayerClass* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Control")
		class UNavigationSystemV1* NavArea;

	virtual bool IsInstigatorValid(AController* EventInstigator) override;

		// Intro Montage
	UFUNCTION(BlueprintCallable, Category = "Intro Control")
		void PlayIntroMontage();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro Stats")
		UAnimMontage* Intro_Montage;
	UFUNCTION(BlueprintCallable, Category = "Intro Control")
		virtual void IntroEndEffect(UAnimMontage* Montage, bool bInterrupted);

	/* Interaction with Player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Detection Control")
		bool CanDetectPlayer = true; /* only true when no combat*/

	UFUNCTION(BlueprintCallable, Category = "Player Detection Control")
		void DetecPlayerOverlap(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FCollisionQueryParams CollisionParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control stats")
		int UnseenRepActionNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control stats")
		int MeleeActionNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control stats")
		int MidRangeActionNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control stats")
		int RangedActionNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control Stats")
		float MeleeRange = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control Stats")
		float MidRange = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Control Stats")
		float RangedRange = 3000.0f;

		/* Actions Control*/
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		FVector GetPlayerLocation();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		float GetPlayerDistance();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		FVector GetPlayerDirection();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		FVector	GetPlayerSameHeightDirection();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		FVector GetRandomLocation(FVector IniPosition, float Distance);
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		bool LocationValidForTP(FVector TPLocation);
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		int GetRandomInRangeNum(int max);

		/* Common/ Default Actions*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Reposition Action")
		bool IsDoingAnyAction = false; /* The general bool that control BT*/

	// Reposition is just set a FVector to controller to BT
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		bool NeedToUnseenReposition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Reposition Action Stats")
		float RepositionFrontDistance = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Reposition Action Stats")
		float RepositionBackDistance = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Reposition Action Stats")
		float RepositionLeftDistance = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Reposition Action Stats")
		float RepositionRightDistance = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Default Reposition Action")
		FVector GetBackwardRepositionLocation();
	UFUNCTION(BlueprintCallable, Category = "Default Reposition Action")
		FVector GetForwardRepositionLocation();
	UFUNCTION(BlueprintCallable, Category = "Default Reposition Action")
		FVector GetLeftRepositionLocation();
	UFUNCTION(BlueprintCallable, Category = "Default Reposition Action")
		FVector GetRightRepositionLocation();

		// Player Position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Reposition Action Stats")
		float CloseMainDistance = 200.0f;
	UFUNCTION(BlueprintCallable, Category = "Default Reposition Action")
		FVector GetPlayerFrontPosition();
	UFUNCTION(BlueprintCallable, Category = "Default Reposition Action")
		FVector GetPlayerBackPosition();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Action Control")
		bool bLookingAtPlayer = false;
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		void LookAtPlayer();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		void TurnToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		bool GetDoingAnyAction();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		void FinishAction();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		void StopAtLookingPlayer();

	UFUNCTION(BlueprintCallable, Category = "MoveTo Controller")
		void WrapperMoveToAttackOffset(FVector Location);
	UFUNCTION(BlueprintCallable, Category = "MoveTo Controller")
		virtual void MoveToEnd();

	UFUNCTION(BlueprintCallable, Category = "Override Reposition Action")
		virtual void UnseenRepositionAction();
	
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		virtual void SelectActionToDo();
	virtual void DoActionWithOffset(int OriginalChance, TFunction<void()> OriginalAction,
		TFunction<void()> SecondaryAction);
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		void AceptAction();
	UFUNCTION(BlueprintCallable, Category = "Default Action Control")
		void RegreatAction();

	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		virtual void MeleeAction();
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		virtual void MidRangeAction();
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		virtual void RangedAction();

	UFUNCTION(BlueprintCallable, Category = "Override Extra Action")
		virtual void FollowActionOne();
	UFUNCTION(BlueprintCallable, Category = "Override Extra Action")
		virtual void FollowActionTwo();

	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
};

