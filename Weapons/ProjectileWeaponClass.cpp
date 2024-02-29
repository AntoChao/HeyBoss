// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectileWeaponClass.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Math/UnrealMathUtility.h"


AProjectileWeaponClass::AProjectileWeaponClass(){
	// Collision Component
	WeaponCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Coliision Comp"));

	WeaponCollisionComp->SetOnlyOwnerSee(false);
	WeaponCollisionComp->bCastDynamicShadow = true;
	WeaponCollisionComp->CastShadow = true;
	WeaponCollisionComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	WeaponCollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	WeaponCollisionComp->CanCharacterStepUpOn = ECB_No;
	WeaponCollisionComp->SetCollisionResponseToAllChannels(ECR_Block);

	RootComponent = WeaponCollisionComp;

	RootSceneComponent->SetupAttachment(WeaponCollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>
						(TEXT("WeaponProjectileMovement"));

}

void AProjectileWeaponClass::BeginPlay() {
	Super::BeginPlay();

	// overlap event
	if (IsValid(WeaponCollisionComp)) {
		WeaponCollisionComp->OnComponentHit.AddDynamic(this,
			&AProjectileWeaponClass::OnHit);
	}

    SetInitProjectile();
}

void AProjectileWeaponClass::SetInitProjectile() {
    if (ProjectileMovement) {

        // Configure movement properties
        SetLifeSpan(PorjectileLifeSpan);
        ProjectileMovement->InitialSpeed = InitProjectileSpeed;
        ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
        ProjectileMovement->bRotationFollowsVelocity = ProjectilebRotationFollowsVelocity;
        ProjectileMovement->bShouldBounce = ProjectilebShouldBounce;
        ProjectileMovement->Bounciness = ProjectileBounciness;
        ProjectileMovement->ProjectileGravityScale = ProjectileGravityScale;
        ProjectileMovement->HomingAccelerationMagnitude = ProjectileAccelerationMagnitude;

        IsActivated = true;
    }
}

// testing, the direction is safenormal. May need to multiply to initialspeed
void AProjectileWeaponClass::SetProjectileDirection(FVector Direction) {

	DrawDebugLine(GetWorld(), GetActorLocation(), Direction * InitProjectileVelocity, 
		FColor::Red, false, 10, 0, 5);

    ProjectileMovement->Velocity = Direction * InitProjectileVelocity;
}

void AProjectileWeaponClass::SetWeaponTarget(ACharacterClass* NewTarget) {
	WeaponTarget = NewTarget;
}

void AProjectileWeaponClass::FlyToTarget() {

	if (IsValid(WeaponTarget)) {
		
		FVector Direction = (WeaponTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		DrawDebugLine(GetWorld(), GetActorLocation(), Direction * InitProjectileVelocity,
			FColor::Red, false, 10, 0, 5);

		ProjectileMovement->Velocity = Direction * InitProjectileVelocity;
	}
}

	// Delay
void AProjectileWeaponClass::DelayFlyToTarget(float DelayTime) {
	GetWorldTimerManager().SetTimer(FlyTimerHandle, this,
		&AProjectileWeaponClass::FlyToTarget, DelayTime, false);
}

// Only call Hit if hit character or my environment
void AProjectileWeaponClass::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		
		// TESTING
		/*
		UCapsuleComponent* HittenCompCap = Cast<UCapsuleComponent>(OtherComp);
		if (HittenCompCap) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("IS HITTEN A CAPSULE COMPONENT"));
		}
		else {
			USkeletalMeshComponent* HittenCompSkele = Cast<USkeletalMeshComponent>(OtherComp);
			if (HittenCompSkele) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("IS HITTEN A SEKELTON COMPONENT"));

			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("IS HITTEN A DIFFENT COMPONENT"));

			}
		}*/
		
		if (OtherActor->GetClass()->IsChildOf(ACharacterClass::StaticClass())) {
			if (OwnByPlayer) {
				AEnemyClass* enemy = Cast<AEnemyClass>(OtherActor);

				if (IsValid(enemy)) {
					ApplyOverlapDamage(enemy, Hit);	
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PROJECTILE HIT ENEMY"));
				}
			}
			else {
				APlayerClass* player = Cast<APlayerClass>(OtherActor);

				if (IsValid(player)) {
					ApplyOverlapDamage(player, Hit);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PROJECTILE HIT PLAYER"));
				}
			}
		}
		else if (OtherActor->GetClass()->IsChildOf(AGeneralMapComponent::StaticClass())) {
			PlayHitEnvironmentEffect(Hit);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PROJECTILE HIT ENVIRONMENT"));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PROJECTILE HIT OTHER THING"));
		}
		Destroy();
		
	}
}