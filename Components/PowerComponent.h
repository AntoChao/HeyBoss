// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UPowerComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPowerComponent();

protected:
	/* Basic Stats*/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Power = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CritChance = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CritMultipier = 1.0f;
public:
	/* Settersand Getters*/ 
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		void SetPower(float PowerValue);

	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		float GetPower();

	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		void SetCritChance(float critChanceValue);

	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		float GetCritChance();

	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		void SetCritMultiplier(float critMultiplierValue);

	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		float GetCritMultiplier();

	/* Updates Values*/ 
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateFlatPower(float modifier);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdatePercentagePower(float modifier);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateCritChance(float modifier);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateCritMultiplier(float modifier);

};

