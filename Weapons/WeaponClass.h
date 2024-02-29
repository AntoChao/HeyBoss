// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../World/Common_Enums.h"
#include "Kismet/GameplayStatics.h"

// Class to recognize
#include "../Characters/CharacterClass.h"
#include "../Characters/Player/PlayerClass.h"
#include "../Characters/Player/CustomPlayerController.h"
#include "../Characters/Enemies/EnemyClass.h"
#include "../Characters/Enemies/EnemyController.h"

#include "../Map/GeneralMapComponent.h"
// components
#include "../Components/PowerComponent.h"

// sound
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// niagara
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

// Timer
#include "TimerManager.h"

#include "WeaponClass.generated.h"

UCLASS()
class AWeaponClass : public AActor {
	GENERATED_BODY()

public:
	AWeaponClass();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Components*/
		// Scene Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Component")
		class USceneComponent* RootSceneComponent = nullptr;
		// Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Component")
		class UStaticMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FVector LeftHandleLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FVector RightHandleLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FTransform FireTrans;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FName Left_Handle_Socket = FName("Socket_LeftHandle");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FName Right_Handle_Socket = FName("Socket_RightHandle");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FName Fire_Socket = FName("Socket_Fire");

	/* Owner/ Controller*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Controller")
		ACharacterClass* WeaponOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Controller")
		class AController* OwnerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Controller")
		bool OwnByPlayer = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		void SetWeaponController(ACharacterClass* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		FVector GetWeaponLeftHandleLocation();

	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		FVector GetWeaponRightHandleLocation();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		FTransform GetWeaponFireSocketTransform();

	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		virtual void Activate(FVector TargetLocation);
	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		virtual void Desactivate();

	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		virtual bool IsAbleToActivate();
	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		virtual void ReloadAmmo();

	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		virtual int GetCurrentAmmo();
	UFUNCTION(BlueprintCallable, Category = "Weapon Controller")
		virtual void SetCurrentAmmo(int amount);
};



