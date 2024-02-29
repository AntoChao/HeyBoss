// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectWeaponClass.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ActorComponent.h"

#include "ProjectileWeaponClass.generated.h"

/* Projectile Weapon*/

UCLASS()
class AProjectileWeaponClass : public ADirectWeaponClass
{
	GENERATED_BODY()

public:
	AProjectileWeaponClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Component*/

	// Collision Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapom Collision Component")
		class UBoxComponent* WeaponCollisionComp;

	UFUNCTION(BlueprintCallable, Category = "Weapom Collision Component")
		virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			const FHitResult& Hit);

	// Projectile Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component")
		class UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float PorjectileLifeSpan = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float InitProjectileSpeed = 100000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float ProjectileMaxSpeed = 300000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		bool ProjectilebRotationFollowsVelocity = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		bool ProjectilebShouldBounce = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float ProjectileBounciness = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float ProjectileGravityScale = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float ProjectileAccelerationMagnitude = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Component Stats")
		float InitProjectileVelocity = 100000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Scene Stats")
		class UNiagaraSystem* ProjectileTailNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FName Tail_Socket = FName("Socket_Trail1");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Skeleton Control Stats")
		FName Tail_Socket2 = FName("Socket_Trail2");

	UFUNCTION(BlueprintCallable, Category = "Projectile Component")
		virtual void SetInitProjectile();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Projectile Niagara Component")
		void SetProjectileTrail();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Controller")
		ACharacterClass* WeaponTarget = nullptr;
public:
	// Velocity/ Direction
	UFUNCTION(BlueprintCallable, Category = "Projectile Component")
		void SetProjectileDirection(FVector Direction);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Projectile Component")
		void SetProjectileIgnoreOwner();

	UFUNCTION(BlueprintCallable, Category = "Projectile Target Controller")
		void SetWeaponTarget(ACharacterClass* NewTarget);
	UFUNCTION(BlueprintCallable, Category = "Projectile Target Controller")
		void FlyToTarget();

		// Maybe delay the fly target
	UFUNCTION(BlueprintCallable, Category = "Projectile Target Controller")
		void DelayFlyToTarget(float DelayTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Target Controller")
		FTimerHandle FlyTimerHandle;
};



