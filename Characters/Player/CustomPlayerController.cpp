// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACustomPlayerController::ACustomPlayerController() {
}

void ACustomPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer())) {
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	MainPlayer = Cast<APlayerClass>(GetPawn());
}

void ACustomPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		// Game Pause ESC
		EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, this, &ACustomPlayerController::PauseGameCTR);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::LookCTR);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::MoveCTR);

		// Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::RunCTR);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ACustomPlayerController::RunEndCTR);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::JumpCTR);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACustomPlayerController::JumpStopCTR);

		// Dashing/ Rolling
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::DashCTR);

		// Weapon -> Shoting/ Blade Slash
		// EnhancedInputComponent->BindAction(UseWeaponPrimaryAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::UseWeaponPrimaryCTR);
		EnhancedInputComponent->BindAction(UseWeaponPrimaryAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::UseWeaponPrimaryCTR);
		EnhancedInputComponent->BindAction(UseWeaponPrimaryAction, ETriggerEvent::Completed, this, &ACustomPlayerController::StopUsingWeaponPrimaryCTR);

		// Weapon -> Aiming/ Blocking
		EnhancedInputComponent->BindAction(UseWeaponSecondaryAction, ETriggerEvent::Started, this, &ACustomPlayerController::UseWeaponSecondaryCTR);
		EnhancedInputComponent->BindAction(UseWeaponSecondaryAction, ETriggerEvent::Completed, this, &ACustomPlayerController::StopUsingWeaponSecondaryCTR);
	
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ACustomPlayerController::ReloadCTR);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACustomPlayerController::InteractCTR);
	}
}

void ACustomPlayerController::PauseGameCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		if (Value.Get<bool>()) {
			MainPlayer->PauseGame();
		}
	}
}

void ACustomPlayerController::LookCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		FVector2D LookVector = Value.Get<FVector2D>();
		MainPlayer->Look(LookVector);
	}
}

void ACustomPlayerController::MoveCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		FVector2D MoveVector = Value.Get<FVector2D>();
		MainPlayer->Move(MoveVector);
	}
}

void ACustomPlayerController::RunCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->Run();
	}
}
void ACustomPlayerController::RunEndCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->StopRun();
	}
}

/*May need to reorganize the jump*/
void ACustomPlayerController::JumpCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->Jump();
	}
}
void ACustomPlayerController::JumpStopCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->StopJumping();
	}
}

void ACustomPlayerController::DashCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->Dash();
	}
}
/*
void ACustomPlayerController::DashStopCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->StopDash();
	}
}*/

void ACustomPlayerController::UseWeaponPrimaryCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->UseWeaponPrimary();
	}
}
void ACustomPlayerController::StopUsingWeaponPrimaryCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->StopUsingWeaponPrimary();
	}
}
void ACustomPlayerController::UseWeaponSecondaryCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		
		MainPlayer->UseWeaponSecondary();
	}
}
void ACustomPlayerController::StopUsingWeaponSecondaryCTR(const FInputActionValue& Value) {
	/*
	if (IsValid(MainPlayer)) {
		if (MainPlayer->IsAbleToUseWeapon()) {
			MainPlayer->StopUsingWeaponSecondary();
		}
	}*/
	if (IsValid(MainPlayer)) {
		MainPlayer->StopUsingWeaponSecondary();
	}
}

void ACustomPlayerController::ReloadCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		MainPlayer->ManualReloadWeapon();
	}
}

void ACustomPlayerController::InteractCTR(const FInputActionValue& Value) {
	if (IsValid(MainPlayer)) {
		if (MainPlayer->GetIsAbleToInteract()) {
			MainPlayer->InteractEnvironment();
		}
	}
}
