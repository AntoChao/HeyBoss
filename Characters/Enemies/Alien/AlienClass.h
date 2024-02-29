 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyClass.h"
#include "../EnemyController.h"

#include "AlienUFOClass.h"
#include "AlienBloomClass.h"

#include "AlienClass.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AAlienClass : public AEnemyClass
{
	GENERATED_BODY()

public:
	AAlienClass();

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	// Body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Component")
		class USphereComponent* BarrierCollisionComp;

	UFUNCTION(BlueprintCallable, Category = "Collision Component")
		void OnBarrierOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			bool bFromSweep, const FHitResult& SweepResult);

	/* Intro Scene*/
	UFUNCTION(BlueprintCallable, Category = "Intro Scene Stats")
		void StartIntroScene();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro Scene Stats")
		TSubclassOf<class AAlienUFOClass> IntroUFOClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro Scene Stats")
		TSubclassOf<class AAlienBloomClass> IntroBloomClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro Scene Stats")
		FVector IntroSpawnOffSet = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intro Scene Stats")
		FRotator IntroSpawnExtraRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Weapon -> Body
	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void AttachBodyWeapons();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName LHSocket = FName("LHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName RHSocket = FName("RHSocket");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName FootSocket = FName("FootSocket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		FName RHDBSocket = FName("RHDBSocket");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		TSubclassOf<class AMeleeWeaponClass> BodyWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* LeftHand = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* RightHand = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Control Stats")
		class AMeleeWeaponClass* Foot = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Weapon Control")
		void ActivateWeapon();
	
	/* Override Actions -> Called in BT*/
	virtual void SelectActionToDo() override;

	/* Reposition*/
		//TELEPORT
	UFUNCTION(BlueprintCallable, Category = "Override Reposition Action")
		FVector GetTPBackwardLocation();
	UFUNCTION(BlueprintCallable, Category = "Override Reposition Action")
		FVector GetTPForwardLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Action Stats")
		float MoveToPortalTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Action Stats")
		float RepositionTPDistance = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Action Stats")
		UAnimMontage* TP_Montage;
	UFUNCTION(BlueprintCallable, Category = "Override Reposition Action")
		void TeleportAroundPlayer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Action Stats")
		float TeleportRadius = 1000.0f;

		// TP Portal
	UFUNCTION(BlueprintCallable, Category = "Override Reposition Action")
		void SpawnPortal(FVector SpawnLocation, bool Activate, FVector TPTarget);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Stats")
		float PortalHeight = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Stats")
		float PortalDist = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		TSubclassOf<class AAlienPortalClass> PortalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Stats")
		USoundCue* PassPortalSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Reposition Stats")
		class UNiagaraSystem* PortalNiagara;

	/* Melee*/
		// Smash Ground
	UFUNCTION(BlueprintCallable, Category = "Override Melee Action")
		void SmashGround();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Melee Action Stats")
		UAnimMontage* SmashGround_Montage;

	/* Mid RANGE*/
		// Mid Range Kick
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void Kick();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		UAnimMontage* Kick_Montage;

		// TP Punch
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void TPPunch();
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void TPFrontPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		UAnimMontage* TPPunch_Montage;

		// Activate the bullet barrier
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void ActivateBarrier();
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void BarrierActivated();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Override MidRange Action")
		void SpawnAndAttachBarrier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		UAnimMontage* ActivateBarrier_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action")
		bool IsBarrierActivated = false;
	UFUNCTION(BlueprintCallable, Category = "Override MidRange Action")
		void ControlBullet(AProjectileWeaponClass* Bullet);

			// Barrier timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action")
		FTimerHandle BarrierTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override MidRange Action Stats")
		float BarrierActivateTime = 10.0f;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Override MidRange Action")
		void DesactivateBarrier();

	/* Range*/
		// Spawn UFOs 
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void UFOInvation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		UAnimMontage* SpawnUFOs_Montage;

	// UFO Spawn Timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action")
		bool IsSpawningUFO = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller")
		FTimerHandle SpawnUFOTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Controller Stats")
		float UFOSpawnCooldown = 1.5f;

	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void SpawnUFOs();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action")
		int CurrentUFOAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		int UFOAmount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		float UFOSpawnHeight = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		float UFOSpawnRadius = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		TSubclassOf<class AAlienUFOClass> UFOClass;

	

		// DragonBall Projectile
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void DragonBallLazer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		UAnimMontage* DragonBallLazer_Montage;

	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void PrepareDBball();
	UFUNCTION(BlueprintCallable, Category = "Override Range Action")
		void LaunchDBball();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Override Range Action")
		void IgnoreDBBall(AProjectileWeaponClass* ADBBall);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action Stats")
		TSubclassOf<class AProjectileWeaponClass> DBProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Range Action")
		class AProjectileWeaponClass* DBBall = nullptr;

public:
	virtual void UnseenRepositionAction() override;
	virtual void MeleeAction() override;
	virtual void MidRangeAction() override;
	virtual void RangedAction() override;

	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
};

