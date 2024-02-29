// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeleeWeaponClass.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"


AMeleeWeaponClass::AMeleeWeaponClass(){
}

void AMeleeWeaponClass::BeginPlay() {
	Super::BeginPlay();
}

void AMeleeWeaponClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

    if (IsActivated) {
        if (!AlreadyHit) {
            CheckCurrentHits();
        }
    }
}

void AMeleeWeaponClass::SetPreviousLocations() {
	for (int i = 0; i < AmountOfSockets; i++) {
		FName CuurentSocketName = WeaponSockets[i];
		if (WeaponMesh->DoesSocketExist(CuurentSocketName)) {
            PreviousLocations.Add(WeaponMesh->GetSocketLocation(CuurentSocketName));
		}
	}
}

void AMeleeWeaponClass::CheckCurrentHits() {
	for (int i = 0; i < AmountOfSockets; i++) {
		if (WeaponMesh->DoesSocketExist(WeaponSockets[i])) {
			
			CollisionParams.AddIgnoredActor(this);
			CollisionParams.AddIgnoredActor(WeaponOwner);

			// Fire a hit
            FHitResult SweepHitResult;
            FVector CurrentLocation = WeaponMesh->GetSocketLocation(WeaponSockets[i]);
            FVector PreviouLocation = PreviousLocations[i];

            bool bHit = GetWorld()->SweepSingleByChannel(
                SweepHitResult,
                PreviouLocation,
                CurrentLocation,
                FQuat::Identity,
				ECC_Pawn,
                FCollisionShape::MakeSphere(SweepRadius),
                CollisionParams);

			DrawDebugLine(GetWorld(), PreviouLocation, CurrentLocation, FColor::Green, false, 1, 0, 3);

            if (bHit) {
				ValidateHitResult(SweepHitResult);
            }
		}
	}
}

void AMeleeWeaponClass::Activate(FVector TargetLocation) {
	Super::Activate(TargetLocation);
    AlreadyHit = false;
	SetPreviousLocations();
}
void AMeleeWeaponClass::Desactivate() {
    Super::Desactivate();
    PreviousLocations = {};
}
void AMeleeWeaponClass::ValidateHitResult(FHitResult ASweepHit) {
	ACharacterClass* HitCharacter = Cast<ACharacterClass>(ASweepHit.GetActor());
	if (IsValid(HitCharacter)) {
		if (OwnByPlayer) {
			AEnemyClass* enemy = Cast<AEnemyClass>(HitCharacter);

			if (IsValid(enemy)) {
				AlreadyHit = true;
				DrawDebugPoint(GetWorld(), ASweepHit.ImpactPoint,
					15.0f, FColor::Red, false, 10.0f);
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HIT REGISTERED ENEMY"));

				ApplyOverlapDamage(enemy, ASweepHit);
			}
		}
		else {
			APlayerClass* player = Cast<APlayerClass>(HitCharacter);

			if (IsValid(ASweepHit.GetActor())) {
				DrawDebugPoint(GetWorld(), ASweepHit.GetActor()->GetActorLocation(),
					50.0f, FColor::Red, false, 10.0f);

				if (IsValid(player)) {
					AlreadyHit = true;
					DrawDebugPoint(GetWorld(), ASweepHit.ImpactPoint,
						15.0f, FColor::Red, false, 10.0f);
					// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HIT REGISTERED PLAYER"));

					ApplyOverlapDamage(player, ASweepHit);
				}
			}
		}
	}
	else if (ASweepHit.GetActor()->GetClass()->IsChildOf(AGeneralMapComponent::StaticClass())) {
		PlayHitEnvironmentEffect(ASweepHit);
	}
	else if (ASweepHit.GetActor()->GetClass()->IsChildOf(AWeaponClass::StaticClass())) {
		PlayHitNewWeaponEffect(ASweepHit);
	}
}