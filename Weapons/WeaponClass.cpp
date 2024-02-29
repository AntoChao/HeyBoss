// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeaponClass.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"


AWeaponClass::AWeaponClass(){

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Root"));
	RootComponent = RootSceneComponent;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Body"));
	WeaponMesh->SetupAttachment(RootSceneComponent);
}

void AWeaponClass::BeginPlay() {
	Super::BeginPlay();

}

/* Owner Controller*/
void AWeaponClass::SetWeaponController(ACharacterClass* NewOwner) {
	if (IsValid(NewOwner)) {
		WeaponOwner = NewOwner;
		
		if (IsValid(NewOwner->GetController())) {
			OwnerController = NewOwner->GetController();
		}
		
		if (NewOwner->IsA(APlayerClass::StaticClass())) {
			OwnByPlayer = true;
		}
		else {
			OwnByPlayer = false;
		}
	}
}

FVector AWeaponClass::GetWeaponLeftHandleLocation() {
	if (WeaponMesh->DoesSocketExist(Left_Handle_Socket)) {
		LeftHandleLocation = WeaponMesh->GetSocketLocation(Left_Handle_Socket);
	}
	return LeftHandleLocation;
}

FVector AWeaponClass::GetWeaponRightHandleLocation() {
	if (WeaponMesh->DoesSocketExist(Right_Handle_Socket)) {
		RightHandleLocation = WeaponMesh->GetSocketLocation(Right_Handle_Socket);
	}
	return RightHandleLocation;
}

FTransform AWeaponClass::GetWeaponFireSocketTransform() {
	if (WeaponMesh->DoesSocketExist(Fire_Socket)) {
		FireTrans = WeaponMesh->GetSocketTransform(Fire_Socket, 
			ERelativeTransformSpace::RTS_World);
	}
	return FireTrans;
}

void AWeaponClass::Activate(FVector TargetLocation) {
	return;
}
void AWeaponClass::Desactivate() {
	return;
}

// Indirect weapon function, just virtual and be overrided
bool AWeaponClass::IsAbleToActivate() {
	return true;
}

void AWeaponClass::ReloadAmmo() {
	return;
}

int AWeaponClass::GetCurrentAmmo() {
	return 0;
}

void AWeaponClass::SetCurrentAmmo(int amount) {
	return;
}