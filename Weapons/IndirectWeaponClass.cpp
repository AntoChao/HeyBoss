// Copyright Epic Games, Inc. All Rights Reserved.

#include "IndirectWeaponClass.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Math/UnrealMathUtility.h"

#include "ProjectileWeaponClass.h"

AIndirectWeaponClass::AIndirectWeaponClass(){
}

void AIndirectWeaponClass::BeginPlay() {
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}

// activate -> fire 

void AIndirectWeaponClass::Activate(FVector TargetLocation) {
	WeaponFire(TargetLocation);
}

void AIndirectWeaponClass::Desactivate() {
	return;
}


// check if there is enough ammo
bool AIndirectWeaponClass::IsAbleToActivate() {
	return CurrentAmmo > 0;
}

void AIndirectWeaponClass::WeaponFire(FVector TargetLocation) {
	SpawnBullet(TargetLocation);
	WeaponFireExtraAction();
}

void AIndirectWeaponClass::SpawnBullet(FVector TargetLocation) {
	
	// Spawn Bullet
	if (WeaponMesh->DoesSocketExist(Fire_Socket)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransFireComp = WeaponMesh->GetSocketTransform(Fire_Socket,
			ERelativeTransformSpace::RTS_World);

		if (IsValid(ProjectileClass)) {
			AProjectileWeaponClass* Bullet = GetWorld()->SpawnActor<AProjectileWeaponClass>
				(ProjectileClass, TransFireComp.GetLocation(), GetActorRotation(), ActorSpawnParams);

			if (IsValid(Bullet)) {
				Bullet->SetWeaponController(WeaponOwner);
				DrawDebugPoint(GetWorld(), TargetLocation, 10.0f, FColor::Red, false, 2.0f);

				// Use ammo
				CurrentAmmo -= 1;

				// Set Bullet Velocity
				FVector FireDirection = (TargetLocation - TransFireComp.GetLocation()).GetSafeNormal();
				Bullet->SetProjectileDirection(FireDirection);
			}
		}
	}
}

void AIndirectWeaponClass::ReloadAmmo() {
	CurrentAmmo = MaxAmmo;
}

int AIndirectWeaponClass::GetCurrentAmmo() {
	return CurrentAmmo;
}
void AIndirectWeaponClass::SetCurrentAmmo(int amount) {
	CurrentAmmo = amount;
}

void AIndirectWeaponClass::WeaponFireExtraAction() {
	if (WeaponMesh->DoesSocketExist(Fire_Socket)) {
		FTransform TransFireComp = WeaponMesh->GetSocketTransform(Fire_Socket,
			ERelativeTransformSpace::RTS_World);

		// Spawn Sound
		if (IsValid(WeaponFireSoundCue)) {
			UGameplayStatics::PlaySoundAtLocation(this, WeaponFireSoundCue,
				TransFireComp.GetLocation());
		}

		// Spawn Niagara
		if (IsValid(WeaponFireNiagara)) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				WeaponFireNiagara,
				TransFireComp.GetLocation(),
				TransFireComp.Rotator(),
				TransFireComp.GetScale3D(),
				true,
				true,
				ENCPoolMethod::AutoRelease,
				true
			);
		}
	}
}
