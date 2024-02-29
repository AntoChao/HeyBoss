// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterClass.h"
#include "../Enemies/EnemyController.h"
#include "InputActionValue.h"
#include "PlayerWidget.h"

#include "PlayerClass.generated.h"


UCLASS(BlueprintType, Blueprintable)
class APlayerClass : public ACharacterClass
{
	GENERATED_BODY()

public:
	APlayerClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	UFUNCTION(BlueprintCallable, Category = "Component")
		FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable, Category = "Component")
		FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	/* Camera*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* FollowCamera;

	// Run a timeline that smoothly change camera position
	UFUNCTION(BlueprintCallable, Category = "Camera Component")
		void InitializeCameraStats();

	UFUNCTION(BlueprintCallable, Category = "Camera Component")
		void UpdateCameraPosition(FVector NewLocation);
	UFUNCTION(BlueprintCallable, Category = "Camera Component")
		void CameraRepositionFinished();

	FTimeline AimCameraTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Component Stats")
		class UCurveVector* AimCameraCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Component Stats")
		class UCurveVector* StopAimCameraCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Component Stats")
		float AimCameraDuration = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Component Stats")
		float NormalCameraViewField = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Component Stats")
		float AimingCameraViewField = 70.0f;

	// Actors to ignore should be player itself and enemies
		// Detection
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera View")
		float ViewDistance = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera View")
		FVector ViewStart = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera View")
		FVector ViewEnd = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera View")
		FHitResult CameraViewHit;

	UFUNCTION(BlueprintCallable, Category = "Camera View")
		void Detect();

		// Die
	virtual void PlayDieMontage() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		UAnimMontage* AnimMontage_RifleDie1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		UAnimMontage* AnimMontage_RifleDie2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Take Damage Stats")
		UAnimMontage* AnimMontage_RifleDie3;

	UFUNCTION(BlueprintCallable, Category = "Take Damage")
		void PlayRifleDieMontage();

public: 
	/* Take Damage*/
	virtual bool IsInstigatorValid(AController* EventInstigator) override;
	virtual bool IsAbleToTakeDamage() override;

	// Die
	virtual void ActorDie() override; /*Destroy + others*/

	// Controller
	UFUNCTION(BlueprintCallable, Category = "Control")
		void InitializeController();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class ACustomPlayerController* PlayerController;

		// Pause game Controller
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Controller")
		void PauseGame();
	
		// Look Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller")
		bool IsAbleToLook = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
		float Look_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look Controller Stats")
		float Look_YSensitivity = 1.0f;
	UFUNCTION(BlueprintCallable, Category = "Controller Look")
		void Look(FVector2D LookAxisVector);

		/* Special Controller*/
	UFUNCTION(BlueprintCallable, Category = "Controller Move")
		void LockMovement();
	UFUNCTION(BlueprintCallable, Category = "Controller Move")
		void UnlockMovement();

		// Move Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller")
		bool IsAbleToMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
		float Move_XSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move Controller Stats")
		float Move_YSensitivity = 1.0f;
	UFUNCTION(BlueprintCallable, Category = "Controller Move")
		void Move(FVector2D MovementVector);
		
		// Run Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Controller")
		bool IsRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run Controller")
		bool IsAbleToRun = true;
	UFUNCTION(BlueprintCallable, Category = "Run Controller")
		void Run();
	UFUNCTION(BlueprintCallable, Category = "Run Controller")
		void StopRun();
	
		// Jump Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Controller")
		bool IsJumpping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Controller")
		bool IsAbleToJump = true;

	virtual void Jump() override;
	virtual void StopJumping() override;

		// Dash Controller
	UFUNCTION(BlueprintCallable, Category = "Dash Control")
		void InitializeDashStats();

		// Curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control")
		bool IsDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control")
		bool IsAbleToDash = true;
	FTimeline DashTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control Stats")
		class UCurveFloat* DashCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control Stats")
		float MaxDashDistance = 750.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control Stats")
		float MaxDashDuration = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control")
		FHitResult DashCollideHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control Stats")
		FVector DashInitLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Control Stats")
		FVector DashTargetLocation = FVector::ZeroVector;

	UFUNCTION(BlueprintCallable, Category = "Dash Controller")
		void Dash();
	UFUNCTION(BlueprintCallable, Category = "Dash Controller")
		void UpdateDashLocation(float Value);
	UFUNCTION(BlueprintCallable, Category = "Dash Controller")
		void DashEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Dash Controller")
		void PlayDashEffects();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Dash Controller")
		void StopDashEffects();
	
		// Dash Timer for cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller")
		FTimerHandle DashTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller Stats")
		float DashCooldown = 0.5f;
	UFUNCTION(BlueprintCallable, Category = "Jump Controller")
		void ResetDashAvaibility();

		// Weapon Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control")
		class AWeaponClass* PrimaryWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control")
		bool IsFiring = false;
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void UpdateAimingTheRotation();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		bool IsAbleToUseWeapon();

		// Weapon Primary
		// Should be call in animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control")
		bool AbleToUseWeapon = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control")
		bool IsUsingWeaponPrimary = false;
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void UseWeaponPrimary();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ActivateWeaponPrimary();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void DesactivateWeaponPrimary();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void StopUsingWeaponPrimary();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ReloadWeaponPrimary();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ManualReloadWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		FVector CalculateTargetLocation();

		// Weapon Secondary
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control")
		bool IsBlocking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control")
		bool IsAiming = false;
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void UseWeaponSecondary();
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void StopUsingWeaponSecondary();

		// Block
	UFUNCTION(BlueprintCallable, Category = "Body Control")
		void TryToBlock();
		
		// Aim
	UFUNCTION(BlueprintCallable, Category = "Camera Control")
		void TryToAim();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Control Stats")
		bool CameraInAimingPosition = false;

	/* Player Widget*/
		// Basic Stats
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
		void InitializePlayerHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Control Stats")
		TSubclassOf<class UPlayerWidget> PlayerHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Control Stats")
		class UPlayerWidget* PlayerHUD;

	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
		UPlayerWidget* GetPlayerHUD();

		// Environment/ Player Interaction
	UFUNCTION(BlueprintCallable, Category = "Widget Interact Control")
		void InteractEnvironment();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Interact Control")
		bool IsAbleToInteract = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Interact Control")
		AEnemyClass* InteracTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Interact Control")
		int SceneNumber = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Interact Control")
		int DialogueNumber = 0;

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
		void EnableInteraction(AEnemyClass* Enemy); /* for now, only interact with enemy*/
	UFUNCTION(BlueprintCallable, Category = "Interact Control")
		void DisableInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interact Control")
		bool GetIsAbleToInteract();


	/* Sound*/
		// Floor control
	UFUNCTION(BlueprintCallable, Category = "Sound")
		void SetFloorType(EFloorType NewFloorType);

	/* Enemy Interaction*/
		// Alien
	UFUNCTION(BlueprintCallable, Category = "Sound ")
		void GotUFOSucked();

protected:
	/* Initialize LineTrace Parameters*/
	UFUNCTION(BlueprintCallable, Category = "Detection")
		void InitializeLineTrace();

	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParam;


	/* IMPLEMENT IN FUTURE*/
	/* Body (Sockets) Control*/
	/* Animation Blueprint and weapon system*/
	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void PlaySelectAnimBlueprint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Blueprint Control Stats")
		TSubclassOf<UAnimInstance> KatanaAnimBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Blueprint Control Stats")
		TSubclassOf<UAnimInstance> RifleAnimBlueprint;

	// Animation montage
	virtual void PlaySelectedAnimMontage(UAnimMontage* ChosenMontage, float PlayRate) override;
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	
		// Spawning the weapon
	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void TakeOutWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Blueprint Control Stats")
		UAnimMontage* TakeOutBlade_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Blueprint Control Stats")
		UAnimMontage* TakeOutRifle_Montage;

	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void SpawnAttachPlayerWeapon();

		// Swapping the weapon
	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void ChangeWeapon(EPrimaryWeapon NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void PutBackWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Blueprint Control Stats")
		UAnimMontage* PutBackBlade_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Blueprint Control Stats")
		UAnimMontage* PutBackRifle_Montage;

	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void PutBackWeaponFinished();
	UFUNCTION(BlueprintCallable, Category = "Animation Blueprint Control")
		void DetachDestroyPlayerWeapon();

	/* General Variables for all weapons*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		EPrimaryWeapon PlayerWeapon = EPrimaryWeapon::EBlade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		EPrimaryWeapon NewPlayerWeapon = EPrimaryWeapon::EBlade;

	UFUNCTION(BlueprintCallable, Category = "Player Skeleton Control Stats")
		void UpdatePlayerBodyInfo();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Skeleton Control")
		FVector Player_LHLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Skeleton Control")
		FVector Player_RHLocation = FVector::ZeroVector;

		// Blade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade Weapon Control")
		FName Blade_LHSocket = FName("Blade_LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade Weapon Control")
		FName Blade_RHSocket = FName("Blade_RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade Weapon Control Stats")
		TSubclassOf<class ADirectWeaponClass> BladeWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade Weapon Control Stats")
		UAnimMontage* UseBladePrimary_Montage;

		// Pistol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pistol Weapon Control")
		FName Pistol_LHSocket = FName("Pistol_LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pistol Weapon Control")
		FName Pistol_RHSocket = FName("Pistol_RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pistol Weapon Control Stats")
		TSubclassOf<class AIndirectWeaponClass> PistolWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pistol Weapon Control Stats")
		UAnimMontage* UsePistolPrimary_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pistol Weapon Control Stats")
		UAnimMontage* ReloadPistol_Montage;

		// Shotgun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Weapon Control")
		FName Shotgun_LHSocket = FName("Shotgun_LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Weapon Control")
		FName Shotgun_RHSocket = FName("Shotgun_RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Weapon Control Stats")
		TSubclassOf<class AIndirectWeaponClass> ShotgunWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Weapon Control Stats")
		UAnimMontage* UseShotgunPrimary_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun Weapon Control Stats")
		UAnimMontage* ReloadShotgun_Montage;

		// Rifle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle Weapon Control")
		FName Rifle_LHSocket = FName("Rifle_LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle Weapon Control")
		FName Rifle_RHSocket = FName("Rifle_RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle Weapon Control Stats")
		TSubclassOf<class AIndirectWeaponClass> RifleWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle Weapon Control Stats")
		UAnimMontage* UseRiflePrimary_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle Weapon Control Stats")
		UAnimMontage* ReloadRifle_Montage;

	/* Sound Control*/
	// Floor Sound Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		EFloorType CurrentFloorType = EFloorType::EConcrete;

	// walk sound
	UFUNCTION(BlueprintCallable, Category = "Sound ")
		void PlayWalkFloorSound();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* WalkConcreateSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* WalkGrassSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* WalkDirtSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* WalkSandSoundCue;

	UFUNCTION(BlueprintCallable, Category = "Sound ")
		void PlayJumpFloorSound();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* JumpConcreateSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* JumpGrassSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* JumpDirtSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Stats")
		USoundCue* JumpSandSoundCue;

	/* Enemy Interaction*/
		// Alien
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade Weapon Control Stats")
		UAnimMontage* UFOSuck_Montage;
};

