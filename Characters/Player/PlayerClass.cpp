// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerClass.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Weapon
#include "../../Weapons/WeaponClass.h"
#include "../../Weapons/DirectWeaponClass.h"
#include "../../Weapons/IndirectWeaponClass.h"

#include"CustomPlayerController.h"


/* Debug Tool
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Control stop use weapon secondary"));
DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 2.0f);
DrawDebugLine(GetWorld(), ViewStart, ViewEnd, FColor::Green, false, 1, 0, 1);

FString FloatString = FString::Printf(TEXT("MyFloatValue: %.2f"), MyFloatValue);
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FloatString);
*/

APlayerClass::APlayerClass() {
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

void APlayerClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	// Select Animation Blueprint
	PlaySelectAnimBlueprint();

	// Spawn Weapon
	SpawnAttachPlayerWeapon();

	InitializeLineTrace();
	InitializeCameraStats();
	InitializeDashStats();
	InitializePlayerHUD();
}

void APlayerClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Shot Traces
	Detect();

	// Prob no necesarry
	UpdatePlayerBodyInfo();

	// Change player rotation when is aiming/ shotting
	UpdateAimingTheRotation();

	// Allow timeline ticking
	DashTimeline.TickTimeline(DeltaTime);
	AimCameraTimeline.TickTimeline(DeltaTime);
}

void APlayerClass::InitializeLineTrace() {
	// Ignore actor
	ActorsToIgnore.Add(this);
	DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
}

void APlayerClass::Detect() {
	if (IsValid(FollowCamera)) {
		ViewStart = FollowCamera->GetComponentLocation();
		FVector ViewForwardVector = FollowCamera->GetForwardVector();
		ViewEnd = ((ViewForwardVector * ViewDistance) + ViewStart);

		// DrawDebugLine(GetWorld(), ViewStart, ViewEnd, FColor::Green, false, 1, 0, 1);

		GetWorld()->LineTraceSingleByChannel(CameraViewHit, ViewStart, ViewEnd, 
			ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam);
	}
}

void APlayerClass::InitializeCameraStats() {

	// Setting up aim camera timeline
	AimCameraTimeline.SetTimelineLength(AimCameraDuration);
	AimCameraTimeline.SetLooping(false);

	FOnTimelineEventStatic AimCameraTimelineFinishedCallBack;

	AimCameraTimelineFinishedCallBack.BindUFunction(this, FName("CameraRepositionFinished"));
	AimCameraTimeline.SetTimelineFinishedFunc(AimCameraTimelineFinishedCallBack);
}

void APlayerClass::UpdateCameraPosition(FVector NewLocation) {
	CameraBoom->SocketOffset = NewLocation;
}

void APlayerClass::CameraRepositionFinished() {
	if (IsValid(FollowCamera)) {
		if (IsAiming) {
			FollowCamera->SetFieldOfView(AimingCameraViewField);
		}
		else {
			FollowCamera->SetFieldOfView(NormalCameraViewField);
		}
	}
}

/* Take Damage/ Die*/
void APlayerClass::ActorDie() {
	Super::ActorDie();
	LockMovement();
}

	// Die reaction montage
void APlayerClass::PlayDieMontage() {
	if (IsValid(PrimaryWeapon)) {
		switch (PlayerWeapon) {
			case EPrimaryWeapon::EBlade: {

				break;
			}
			case EPrimaryWeapon::EPistol: {
				break;
			}
			case EPrimaryWeapon::EShotgun: {
				break;
			}
			case EPrimaryWeapon::ERifle: {
				PlayRifleDieMontage();
				break;
			}
			default: {
				break;
			}
		}
	}
}

void APlayerClass::PlayRifleDieMontage() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	int DieSelected = FMath::RandRange(1, 3);

	switch (DieSelected) {
		case 1: {
			AnimInstance->Montage_Play(AnimMontage_RifleDie1, 1.0f);
		break;
		}
		case 2: {
			AnimInstance->Montage_Play(AnimMontage_RifleDie2, 1.0f);
			break;
		}
		case 3: {
			AnimInstance->Montage_Play(AnimMontage_RifleDie3, 1.0f);
			break;
		}
	}

	AnimInstance->OnMontageEnded.AddUniqueDynamic(this,
		&APlayerClass::OnDieMontageEnded);
}



// Controllers
void APlayerClass::InitializeController() {
	PlayerController = Cast<ACustomPlayerController>(GetController());
}

bool APlayerClass::IsInstigatorValid(AController* EventInstigator) {
	AEnemyController* EnemyController = Cast<AEnemyController>(EventInstigator);

	if (IsValid(EnemyController)) {
		return true;
	}
	else{
		return false;
	}
}

bool APlayerClass::IsAbleToTakeDamage() {
	if (!IsDashing) {
		return true;
	}
	else {
		return false;
	}
}

void APlayerClass::InitializeDashStats() {
	// Setting up dash timeline
	FOnTimelineFloat DashTimelineCallBack;
	FOnTimelineEventStatic DashTimelineFinishedCallBack;

	DashTimelineCallBack.BindUFunction(this, FName("UpdateDashLocation"));
	DashTimeline.AddInterpFloat(DashCurve, DashTimelineCallBack);

	DashTimelineFinishedCallBack.BindUFunction(this, FName("DashEnd"));
	DashTimeline.SetTimelineFinishedFunc(DashTimelineFinishedCallBack);
}

void APlayerClass::Look(FVector2D LookAxisVector) {
	if (IsValid(PlayerController)) {
		if (IsAbleToLook) {
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X * Look_XSensitivity);
			AddControllerPitchInput(LookAxisVector.Y * Look_YSensitivity);
		}
	}
}

void APlayerClass::LockMovement() {
	IsAbleToMove = false;
	IsAbleToRun = false;
	IsAbleToJump = false;
	IsAbleToDash = false;
	AbleToUseWeapon = false;
}

void APlayerClass::UnlockMovement() {
	IsAbleToMove = true;
	IsAbleToRun = true;
	IsAbleToJump = true;
	IsAbleToDash = true;
	AbleToUseWeapon = true;
}

void APlayerClass::Move(FVector2D MoveAxisVector) {
	if (IsValid(PlayerController)) {
		if (IsAbleToMove) {
			
			// find out which way is forward
			const FRotator Rotation = PlayerController->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MoveAxisVector.Y * Move_YSensitivity);
			AddMovementInput(RightDirection, MoveAxisVector.X * Move_XSensitivity);
		}
	}
}

// Run
void APlayerClass::Run() {
	if (IsAbleToRun) {
		IsRunning = true;
		CurrentRunSpeed = DefaultRunSpeed;
		ChangeCurrentSpeed();
	}
}

void APlayerClass::StopRun() {
	IsRunning = false;
	CurrentRunSpeed = 0.0f;
	ChangeCurrentSpeed();
}

// Jump
void APlayerClass::Jump() {
	if (IsAbleToJump) {
		Super::Jump();
		IsJumpping = true;
	}
}

void APlayerClass::StopJumping() {
	Super::StopJumping();
	IsJumpping = false;
}

// Dash
void APlayerClass::Dash() {
	if (IsAbleToDash) {		
		if (IsValid(DashCurve)) {
			IsDashing = true;
			IsAbleToDash = false;

			DashInitLocation = GetActorLocation();
			FVector DashDirection = GetActorForwardVector().GetSafeNormal();

			float DashDistance = GetCharacterMovement()->Velocity.Size();
			
			DashTargetLocation = DashInitLocation + DashDirection * DashDistance;

			DrawDebugLine(GetWorld(), GetActorLocation(), DashTargetLocation, FColor::Green, false, 10, 0, 1);

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			
			// Faster the current speed, dash larger with more corresponding time
			float DashDuration = DashDistance * MaxDashDuration / MaxDashDistance;

			DashTimeline.SetTimelineLength(DashDuration);
			DashTimeline.SetLooping(false);

			DashTimeline.PlayFromStart();

			PlayDashEffects();
		}
	}
}

void APlayerClass::UpdateDashLocation(float Value) {
	FVector NewLocation = FMath::Lerp(DashInitLocation, DashTargetLocation, Value);

	bool bHit1 = GetWorld()->LineTraceSingleByChannel(DashCollideHit, 
		GetActorLocation(), NewLocation + FVector(0.0f, 0.0f, BodyHalfHeight/2), ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	bool bHit2 = GetWorld()->LineTraceSingleByChannel(DashCollideHit,
		GetActorLocation(), NewLocation - FVector(0.0f, 0.0f, BodyHalfHeight / 2), ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	bool bHit3 = GetWorld()->LineTraceSingleByChannel(DashCollideHit,
		GetActorLocation(), NewLocation, ECC_Visibility,
		DefaultComponentQueryParams, DefaultResponseParam);

	if (bHit1 || bHit2 || bHit3) {
		DashTimeline.Stop();
		DashEnd();
	}
	else {
		SetActorLocation(NewLocation);
	}
}

void APlayerClass::DashEnd() {
	IsDashing = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	StopDashEffects();

	GetWorldTimerManager().SetTimer(DashTimer, this,
		&APlayerClass::ResetDashAvaibility, DashCooldown, false);
}

void APlayerClass::ResetDashAvaibility() {
	IsAbleToDash = true;
}

/* *******************************/
// CAN BE DELETED????
void APlayerClass::UpdatePlayerBodyInfo() {
	if (IsValid(PrimaryWeapon)) {
		Player_LHLocation = PrimaryWeapon->GetWeaponLeftHandleLocation();
		Player_RHLocation = PrimaryWeapon->GetWeaponRightHandleLocation();
	}
}

void APlayerClass::UpdateAimingTheRotation() {
	if (IsFiring || IsAiming) {
		if (IsValid(PlayerController)) {
			SetActorRotation(FRotator(0.0f, GetController()->GetControlRotation().Yaw, 
				GetController()->GetControlRotation().Roll));
		}
	}
}

// Weapon
/* 
Change Player weapon -> 1: put back weapon based current weapon
						2: change primary weapon enum
						2: play selected animation blueprint
						3: take out weapon
Take Out weapon -> call animation to call spawnattachplayerweapon and call 
					continues change and takeout weapon
Put Back weapon -> call animation to call detachDestroyplayerweapon


*/

/* Animation Blueprint*/
void APlayerClass::PlaySelectAnimBlueprint() {
	if (IsValid(PrimaryWeapon)) {
		switch (PlayerWeapon) {
			case EPrimaryWeapon::EBlade: {
				if (IsValid(KatanaAnimBlueprint)) {
					GetMesh()->SetAnimInstanceClass(KatanaAnimBlueprint);
				}
				break;
			}
			case EPrimaryWeapon::EPistol: {
				// GetMesh()->SetAnimInstanceClass(YourAnimationBlueprintClass);
				break;
			}
			case EPrimaryWeapon::EShotgun: {
				// GetMesh()->SetAnimInstanceClass(YourAnimationBlueprintClass);
				break;
			}
			case EPrimaryWeapon::ERifle: {
				if (IsValid(RifleAnimBlueprint)) {
					GetMesh()->SetAnimInstanceClass(RifleAnimBlueprint);
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

	// call spawn and attach weapon
void APlayerClass::TakeOutWeapon() {
	if (IsValid(PrimaryWeapon)) {
		switch (PlayerWeapon) {
			case EPrimaryWeapon::EBlade: {
				if (IsValid(TakeOutBlade_Montage)) {
					PlaySelectedAnimMontage(TakeOutBlade_Montage, AnimMontagePlayRate);
				}
				break;
			}
			case EPrimaryWeapon::EPistol: {
				// GetMesh()->SetAnimInstanceClass(YourAnimationBlueprintClass);
				break;
			}
			case EPrimaryWeapon::EShotgun: {
				// GetMesh()->SetAnimInstanceClass(YourAnimationBlueprintClass);
				break;
			}
			case EPrimaryWeapon::ERifle: {
				if (IsValid(TakeOutRifle_Montage)) {
					PlaySelectedAnimMontage(TakeOutRifle_Montage, AnimMontagePlayRate);
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

	// Be called by take out animation montage
void APlayerClass::SpawnAttachPlayerWeapon() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (PlayerWeapon) {
		case EPrimaryWeapon::EBlade: {
			if (IsValid(BladeWeaponClass)) {
				FTransform TransLeftHand = GetMesh()->
					GetSocketTransform(Blade_LHSocket, ERelativeTransformSpace::RTS_World);
				FTransform TransRightHand = GetMesh()->
					GetSocketTransform(Blade_RHSocket, ERelativeTransformSpace::RTS_World);

				ADirectWeaponClass* Weapon = GetWorld()->SpawnActor<ADirectWeaponClass>(BladeWeaponClass,
					TransRightHand, ActorSpawnParams);
				PrimaryWeapon = Weapon;

				if (IsValid(PrimaryWeapon)) {
					PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
						Blade_RHSocket);
				}
			}
			break;
		}
		case EPrimaryWeapon::EPistol: {
			if (IsValid(PistolWeaponClass)) {
				FTransform TransLeftHand = GetMesh()->
					GetSocketTransform(Pistol_LHSocket, ERelativeTransformSpace::RTS_World);
				FTransform TransRightHand = GetMesh()->
					GetSocketTransform(Pistol_RHSocket, ERelativeTransformSpace::RTS_World);

				AIndirectWeaponClass* Weapon = GetWorld()->SpawnActor<AIndirectWeaponClass>(PistolWeaponClass,
					TransRightHand, ActorSpawnParams);
				PrimaryWeapon = Weapon;

				if (IsValid(PrimaryWeapon)) {
					PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
						Pistol_RHSocket);
				}
			}
			break;
		}
		case EPrimaryWeapon::EShotgun: {
			if (IsValid(ShotgunWeaponClass)) {
				FTransform TransLeftHand = GetMesh()->
					GetSocketTransform(Shotgun_LHSocket, ERelativeTransformSpace::RTS_World);
				FTransform TransRightHand = GetMesh()->
					GetSocketTransform(Shotgun_RHSocket, ERelativeTransformSpace::RTS_World);

				AIndirectWeaponClass* Weapon = GetWorld()->SpawnActor<AIndirectWeaponClass>(ShotgunWeaponClass,
					TransRightHand, ActorSpawnParams);
				PrimaryWeapon = Weapon;

				if (IsValid(PrimaryWeapon)) {
					PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
						Shotgun_RHSocket);
				}
			}
			break;
		}
		case EPrimaryWeapon::ERifle: {
			if (IsValid(RifleWeaponClass)) {
				FTransform TransLeftHand = GetMesh()->
					GetSocketTransform(Rifle_LHSocket, ERelativeTransformSpace::RTS_World);
				FTransform TransRightHand = GetMesh()->
					GetSocketTransform(Rifle_RHSocket, ERelativeTransformSpace::RTS_World);

				AIndirectWeaponClass* Weapon = GetWorld()->SpawnActor<AIndirectWeaponClass>(RifleWeaponClass,
					TransRightHand, ActorSpawnParams);
				PrimaryWeapon = Weapon;

				if (IsValid(PrimaryWeapon)) {
					PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
						Rifle_RHSocket);
				}
			}
			break;
		}
		default: {
			break;
		}
	}
	PrimaryWeapon->SetWeaponController(this);
}

	// Initializer of all swapping weapon system
void APlayerClass::ChangeWeapon(EPrimaryWeapon NewWeapon) {
	PutBackWeapon();
	NewPlayerWeapon = NewWeapon; /* store the value in a variable*/
}

	// call putbackweapon finished function
void APlayerClass::PutBackWeapon() {
	if (IsValid(PrimaryWeapon)) {
		switch (PlayerWeapon) {
			case EPrimaryWeapon::EBlade: {
				if (IsValid(PutBackBlade_Montage)) {
					PlaySelectedAnimMontage(PutBackBlade_Montage, AnimMontagePlayRate);
				}
				break;
			}
			case EPrimaryWeapon::EPistol: {
				// GetMesh()->SetAnimInstanceClass(YourAnimationBlueprintClass);
				break;
			}
			case EPrimaryWeapon::EShotgun: {
				// GetMesh()->SetAnimInstanceClass(YourAnimationBlueprintClass);
				break;
			}
			case EPrimaryWeapon::ERifle: {
				if (IsValid(PutBackRifle_Montage)) {
					PlaySelectedAnimMontage(PutBackRifle_Montage, AnimMontagePlayRate);
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}
	/* be called by putback weapon
	* destroy player current weapon 
	* set new weapon to weapon
	* change to new animation blueprint
	* take out new weapon
	*/
void APlayerClass::PutBackWeaponFinished() {
	DetachDestroyPlayerWeapon();
	PlayerWeapon = NewPlayerWeapon;
	PlaySelectAnimBlueprint();
	TakeOutWeapon();
}

void APlayerClass::DetachDestroyPlayerWeapon() {
	PrimaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	PrimaryWeapon->Destroy();
	PrimaryWeapon = nullptr;
}


// Just call animation montage
void APlayerClass::UseWeaponPrimary() {
	if (IsValid(PrimaryWeapon)) {
		if (!IsDashing && !IsUsingWeaponPrimary && AbleToUseWeapon) {
			switch (PlayerWeapon) {
				case EPrimaryWeapon::EBlade: {
					if (IsValid(UseBladePrimary_Montage)) {
						PlaySelectedAnimMontage(UseBladePrimary_Montage, AnimMontagePlayRate);
					}
					break;
				}
				case EPrimaryWeapon::EPistol: {
					if (IsAbleToUseWeapon()) {
						if (IsValid(UsePistolPrimary_Montage)) {
							IsFiring = true;
							PlaySelectedAnimMontage(UsePistolPrimary_Montage, AnimMontagePlayRate);
						}
					}
					else {
						if (IsValid(ReloadPistol_Montage)) {
							PlaySelectedAnimMontage(ReloadPistol_Montage, AnimMontagePlayRate);
						}
					}
					break;
				}
				case EPrimaryWeapon::EShotgun: {
					if (IsAbleToUseWeapon()) {
						if (IsValid(UseShotgunPrimary_Montage)) {
							IsFiring = true;
							PlaySelectedAnimMontage(UseShotgunPrimary_Montage, AnimMontagePlayRate);
						}
					}
					else {
						if (IsValid(ReloadShotgun_Montage)) {
							PlaySelectedAnimMontage(ReloadShotgun_Montage, AnimMontagePlayRate);
						}
					}
					break;
				}
				case EPrimaryWeapon::ERifle: {
					if (IsAbleToUseWeapon()) {
						if (IsValid(UseRiflePrimary_Montage)) {
							IsFiring = true;
							PlaySelectedAnimMontage(UseRiflePrimary_Montage, AnimMontagePlayRate);
						}
					}
					else {
						if (IsValid(ReloadRifle_Montage)) {
							PlaySelectedAnimMontage(ReloadRifle_Montage, AnimMontagePlayRate);
						}
					}
					break;
				}
				default: {
					break;
				}
			}
		}
	}
}

// Call In animation montage
void APlayerClass::ActivateWeaponPrimary() {
	if(IsValid(PrimaryWeapon)) {
		PrimaryWeapon->Activate(CalculateTargetLocation());
	}
}

void APlayerClass::DesactivateWeaponPrimary() {
	if(IsValid(PrimaryWeapon)) {
		PrimaryWeapon->Desactivate();
	}
}

void APlayerClass::ReloadWeaponPrimary() {
	if (IsValid(PrimaryWeapon)) {
		PrimaryWeapon->ReloadAmmo();
	}
}

void APlayerClass::ManualReloadWeapon() {
	if (IsValid(PrimaryWeapon)) {
		PrimaryWeapon->SetCurrentAmmo(0);
		UseWeaponPrimary();
	}
}

// ***** SOMETHING WRONG HERE
FVector APlayerClass::CalculateTargetLocation() {
	if (CameraViewHit.bBlockingHit) {
		// A hit occurred, use the hit location
		return CameraViewHit.ImpactPoint;
	}
	else {
		// No hit occurred, use the trace endpoint (end location)
		return CameraViewHit.TraceEnd;
	}
}

void APlayerClass::StopUsingWeaponPrimary() {
	IsFiring = false;
}

void APlayerClass::UseWeaponSecondary() {
	if (IsValid(PrimaryWeapon)) {
		if (!IsDashing && !IsUsingWeaponPrimary && AbleToUseWeapon) {
			switch (PlayerWeapon) {
				case EPrimaryWeapon::EBlade: {
					IsBlocking = true;
					TryToBlock();
					break;
				}
				case EPrimaryWeapon::EPistol: {
					IsAiming = true;
					TryToAim();
					break;
				}
				case EPrimaryWeapon::EShotgun: {
					IsAiming = true;
					TryToAim();
					break;
				}
				case EPrimaryWeapon::ERifle: {
					IsAiming = true;
					TryToAim();
					
					break;
				}
				default: {
					break;
				}
			}
		}
	}
}

// Call by my controller
void APlayerClass::StopUsingWeaponSecondary() {

	IsBlocking = false;
	IsAiming = false;
	TryToAim();
}

bool APlayerClass::IsAbleToUseWeapon() {
	return PrimaryWeapon->IsAbleToActivate();
}

// is there any need??
void APlayerClass::TryToBlock() {
	return;
}
void APlayerClass::TryToAim() {
	FOnTimelineVector AimCameraTimelineCallBack;
	AimCameraTimelineCallBack.BindUFunction(this, FName("UpdateCameraPosition"));
	
	if (IsAbleToUseWeapon()) {
		if (IsAiming) {
			if (IsValid(AimCameraCurve)) {
			
				CameraInAimingPosition = true;
				AimCameraTimeline.AddInterpVector(AimCameraCurve, AimCameraTimelineCallBack);
				AimCameraTimeline.PlayFromStart();
			}
		}
		else {
			if (IsValid(StopAimCameraCurve)) {
			
				CameraInAimingPosition = false;
				AimCameraTimeline.AddInterpVector(StopAimCameraCurve, AimCameraTimelineCallBack);
				AimCameraTimeline.PlayFromStart();
			}
		}
	}
	else {
		if (CameraInAimingPosition) {
			if (IsValid(StopAimCameraCurve)) {
				
				CameraInAimingPosition = false;
				AimCameraTimeline.AddInterpVector(StopAimCameraCurve, AimCameraTimelineCallBack);
				AimCameraTimeline.PlayFromStart();
			}
		}
	}
}

/* Environment Interaction*/
/* Other character has a sphere overlap
*  When Playe overlap -> 1 turn isAbleToInteract true
*						 2 Send its own pointer for player
*  As isAbleToInteract == true, turn on umg and enable controller input
*  Player interact -> turn off all movement, play scripted messages
*  Interact end -> if overlapend or finish interaction
*/

void APlayerClass::InitializePlayerHUD() {
	if (IsValid(PlayerHUDClass)) {
		if (IsValid(PlayerController)) {
			PlayerHUD = CreateWidget<UPlayerWidget>(PlayerController, PlayerHUDClass);
			if (IsValid(PlayerHUD)) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Successfully initialize player widget"));

				PlayerHUD->InitializeWidget(this);
				PlayerHUD->AddToPlayerScreen();
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Unsuccessfully initialize player widget"));
			}
		}
	}
}

UPlayerWidget* APlayerClass::GetPlayerHUD() {
	return PlayerHUD;
}

// play script with on Enemy
void APlayerClass::InteractEnvironment() {
	// Call widget

	// Dialogue plus
	DialogueNumber++;
}

void APlayerClass::EnableInteraction(AEnemyClass* Enemy) {
	IsAbleToInteract = true;
	InteracTarget = Enemy;
	DialogueNumber = 0;
}
void APlayerClass::DisableInteraction() {
	IsAbleToInteract = false;
	InteracTarget = nullptr;
}

bool APlayerClass::GetIsAbleToInteract() {
	return IsAbleToInteract;
}

// Montage 
void APlayerClass::PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance)) {
		if (IsValid(ChosenMontage)) {
			// Variables manipulation
			IsUsingWeaponPrimary = true;

			AnimInstance->Montage_Play(ChosenMontage, PlayRate);

			// Bind the OnMontageEnded event to a custom function
			AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &APlayerClass::OnMontageEnded);
		}
	}
}

void APlayerClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	// UnlockMovement();
	IsUsingWeaponPrimary = false;
}

// Sound Control
void APlayerClass::SetFloorType(EFloorType NewFloorType) {
	CurrentFloorType = NewFloorType;
}

void APlayerClass::PlayWalkFloorSound() {
	switch (CurrentFloorType) {
		case EFloorType::EConcrete: {
			if (IsValid(WalkConcreateSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, WalkConcreateSoundCue, GetActorLocation());
			}
			break;
		}
		case EFloorType::EGrass: {
			if (IsValid(WalkGrassSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, WalkGrassSoundCue, GetActorLocation());
			}
			break;
		}
		case EFloorType::EDirt: {
			if (IsValid(WalkDirtSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, WalkDirtSoundCue, GetActorLocation());
			}
			break;
		}
		case EFloorType::ESand: {
			if (IsValid(WalkSandSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, WalkSandSoundCue, GetActorLocation());
			}
			break;
		}
		default: {
			if (IsValid(WalkConcreateSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, WalkConcreateSoundCue, GetActorLocation());
			}
			break;
		}
	}
}

void APlayerClass::PlayJumpFloorSound() {
	switch (CurrentFloorType) {
		case EFloorType::EConcrete: {
			if (IsValid(JumpConcreateSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, JumpConcreateSoundCue, GetActorLocation());
			}
			break;
		}
		case EFloorType::EGrass: {
			if (IsValid(JumpGrassSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, JumpGrassSoundCue, GetActorLocation());
			}
			break;
		}
		case EFloorType::EDirt: {
			if (IsValid(JumpDirtSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, JumpDirtSoundCue, GetActorLocation());
			}
			break;
		}
		case EFloorType::ESand: {
			if (IsValid(JumpSandSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, JumpSandSoundCue, GetActorLocation());
			}
			break;
		}
		default: {
			if (IsValid(JumpConcreateSoundCue)) {
				UGameplayStatics::PlaySoundAtLocation(this, JumpConcreateSoundCue, GetActorLocation());
			}
			break;
		}
	}
}

/* Enemy Interaction*/
void APlayerClass::GotUFOSucked() {
	if (IsValid(UFOSuck_Montage)) {

		PlaySelectedAnimMontage(UFOSuck_Montage, AnimMontagePlayRate);
		LockMovement();
	}
}
