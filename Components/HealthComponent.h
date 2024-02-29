// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UHealthComponent();

protected:
	// base stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int MaxInt = 2147483647;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float health = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float maxHealth = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool died = false;

	// future implementation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float ExceedHealth = 0.0f;

public:
	// setters and getters
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		void InitHealth(float HP);
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		void SetHealth(float HP);
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		float GetHealth();
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		void SetMaxHealth(float maxHP);
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		float GetMaxHealth();
	UFUNCTION(BlueprintCallable, Category = "Getter && Setter")
		float GetHealthPercentage();
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void SetDied(bool HPState);
	UFUNCTION(BlueprintCallable, Category = "Stats")
		bool GetDied();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateHP(float modifier);
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateByCurrentPercentage(float percentage);
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateByMaxHealthPercentage(float percentage);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateMaxHealth(float value);
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateMaxHealthByPercentage(float percentage);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void RefillMaxHealth();

};

