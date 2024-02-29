// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerClass.h"
#include "CustomPlayerController.generated.h"


UCLASS(BlueprintType, Blueprintable)
class ACustomPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	ACustomPlayerController();

protected:	
	// To add mapping context
	virtual void BeginPlay();

	// APawn interface
	virtual void SetupInputComponent() override;

	// Player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class APlayerClass* MainPlayer;

	/* INPUT SETTING*/ 
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputMappingContext* DefaultMappingContext;

	/* Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* PauseGameAction;

	/* Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* LookAction;

	/* Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* MoveAction;

	/* Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* RunAction;

	/* Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* JumpAction;	
	
	/* Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* DashAction;

	/* Weapon Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* UseWeaponPrimaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* UseWeaponSecondaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* ReloadAction;

	/* Interaction*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* InteractAction;
	
	/* Control*/
	UFUNCTION(BlueprintCallable, Category = "Control")
		void PauseGameCTR(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category = "Control")
		void LookCTR(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category = "Control")
		void MoveCTR(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category = "Control")
		void RunCTR(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
		void RunEndCTR(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
		void JumpCTR(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
		void JumpStopCTR(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
		void DashCTR(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
		void UseWeaponPrimaryCTR(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
		void StopUsingWeaponPrimaryCTR(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
		void UseWeaponSecondaryCTR(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, Category = "Control")
		void StopUsingWeaponSecondaryCTR(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
		void ReloadCTR(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Control")
		void InteractCTR(const FInputActionValue& Value);

};

