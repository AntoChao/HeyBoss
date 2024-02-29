// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectWeaponClass.h"

#include "MeleeWeaponClass.generated.h"

/* Melee Weapon*/

UCLASS()
class AMeleeWeaponClass : public ADirectWeaponClass
{
	GENERATED_BODY()

public:
	AMeleeWeaponClass();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Dectection Stats")
		int AmountOfSockets = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Dectection Stats")
		float SweepRadius = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Dectection Stats")
		TArray<FName> WeaponSockets = {FName("Socket1"), FName("Socket2"), 
									FName("Socket3"), FName("Socket4"), 
									FName("Socket5"), FName("Socket6"),
									FName("Socket7"), FName("Socket8"),
									FName("Socket9"), FName("Socket10")};

	FCollisionQueryParams CollisionParams;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Dectection Stats")
		bool AlreadyHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Dectection Stats")
		TArray<FVector> PreviousLocations;
	
	UFUNCTION(BlueprintCallable, Category = "Hit Detection")
		void SetPreviousLocations();
	UFUNCTION(BlueprintCallable, Category = "Hit Detection")
		void CheckCurrentHits();

	UFUNCTION(BlueprintCallable, Category = "Hit Detection")
		void ValidateHitResult(FHitResult ASweepHit);

public:
	virtual void Activate(FVector TargetLocation) override;
	virtual void Desactivate() override;

	/* Niagara Tail*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon Tail")
		void SpawnAttachTail();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon Tail")
		void DetachDestroyTail();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Tail Stats")
		FName TailSocket1 = FName("TailSocket1");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Tail Stats")
		FName TailSocket2 = FName("TailSocket2");
};



