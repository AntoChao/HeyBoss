// Copyright Epic Games, Inc. All Rights Reserved.

#include "DirectWeaponClass.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"


ADirectWeaponClass::ADirectWeaponClass(){
	// Power Component
	WeaponPowerComponent = CreateDefaultSubobject<UPowerComponent>(TEXT("Power Comp"));
}

void ADirectWeaponClass::BeginPlay() {
	Super::BeginPlay();

	SetInitWeaponPower();
}

void ADirectWeaponClass::Activate(FVector TargetLocation) {
	IsActivated = true;
}
void ADirectWeaponClass::Desactivate() {
	IsActivated = false;
}

/* Power Component*/
void ADirectWeaponClass::SetInitWeaponPower() {
	if (IsValid(WeaponPowerComponent)) {
		WeaponPowerComponent->SetPower(WeaponInitPower);
		WeaponPowerComponent->SetCritChance(WeaponInitCritChance);
		WeaponPowerComponent->SetCritMultiplier(WeaponInitCritMult);
	}
}

	// Damage
float ADirectWeaponClass::CalculateGeneralDamage(UPowerComponent* CharacterPowerComponent) {
	float totalPower = CharacterPowerComponent->GetPower() + WeaponPowerComponent->GetPower();
	float totalCritChance = CharacterPowerComponent->GetPower() + WeaponPowerComponent->GetPower();
	float totalCritMult = CharacterPowerComponent->GetPower() + WeaponPowerComponent->GetPower();

	if (CritictStrike(totalCritChance)) {
		return totalPower * totalCritMult;
	}
	else {
		return totalPower;
	}
}

bool ADirectWeaponClass::CritictStrike(float Probability) {
	float Chance = FMath::RandRange(1, 100);

	return (Chance <= Probability);
}

// IF IS BLADE -> SHOULD HAVE A BOOL THAT REPRESENT IS ATTACKING
	// Apply Damage
void ADirectWeaponClass::ApplyOverlapDamage(ACharacterClass* ACharacter, FHitResult HitResult) {
	if (IsValid(ACharacter)) {
		if (IsValid(WeaponOwner)) {
			if (ACharacter->IsAbleToTakeDamage()) {
				float damage = CalculateGeneralDamage(WeaponOwner->GetCharacterPower());

				FVector AttackDirection = GetActorLocation();

				if (HitResult.IsValidBlockingHit()) {
					AttackDirection = HitResult.ImpactPoint - GetActorLocation();
				}

				ACharacter->SetHitEffectStats(HitResult.ImpactPoint,
					AttackDirection.ToOrientationRotator());

				UGameplayStatics::ApplyPointDamage(ACharacter, damage,
					AttackDirection, HitResult, OwnerController, this, nullptr);

				PlayHitCharacterEffect(HitResult);
			}
		}
	}
}

void ADirectWeaponClass::PlayHitCharacterEffect(FHitResult HitResult) {
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

	// Play sound
	if (IsValid(HitEffectCharacterSoundCue)) {
		UGameplayStatics::PlaySoundAtLocation(this, HitEffectCharacterSoundCue,
			HitResult.ImpactPoint);
	}

	// Play Niagara
	if (IsValid(HitEffectCharacterNiagara)) {
		FVector HitNormal = HitResult.Normal;
		FRotator HitRotation = FRotationMatrix::MakeFromX(HitNormal).Rotator();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffectCharacterNiagara,
			HitResult.ImpactPoint,
			HitRotation,
			FVector(1.0f, 1.0f, 1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
	}
}

void ADirectWeaponClass::PlayHitEnvironmentEffect(FHitResult HitResult) {
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Green, false, 2.0f);

	// Play sound
	if (IsValid(HitEffectEnvironmentSoundCue)) {
		UGameplayStatics::PlaySoundAtLocation(this, HitEffectEnvironmentSoundCue,
			HitResult.ImpactPoint);
	}

	// Play Niagara
	if (IsValid(HitEffectEnvironmentNiagara)) {
		FVector HitNormal = HitResult.Normal;
		FRotator HitRotation = FRotationMatrix::MakeFromX(HitNormal).Rotator();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffectEnvironmentNiagara,
			HitResult.ImpactPoint,
			HitRotation,
			FVector(1.0f, 1.0f, 1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
	}
}

void ADirectWeaponClass::PlayHitNewWeaponEffect(FHitResult HitResult) {
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Yellow, false, 2.0f);

	// Play sound
	if (IsValid(HitNewWeaponSoundCue)) {
		UGameplayStatics::PlaySoundAtLocation(this, HitNewWeaponSoundCue,
			HitResult.ImpactPoint);
	}

	// Play Niagara
	if (IsValid(HitNewWeaponNiagara)) {
		FVector HitNormal = HitResult.Normal;
		FRotator HitRotation = FRotationMatrix::MakeFromX(HitNormal).Rotator();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitNewWeaponNiagara,
			HitResult.ImpactPoint,
			HitRotation,
			FVector(1.0f, 1.0f, 1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
	}
}