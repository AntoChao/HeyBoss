// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeneralMapComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"


AGeneralMapComponent::AGeneralMapComponent(){

	BuildingSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = BuildingSceneComponent;

	// Use a sphere as a simple collision representation
	BuildingCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Building Area"));
	BuildingCollisionComp->SetupAttachment(BuildingSceneComponent);

	BuildingBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building"));
	BuildingBody->SetOnlyOwnerSee(false);
	BuildingBody->SetupAttachment(BuildingSceneComponent);
	BuildingBody->bCastDynamicShadow = false;
	BuildingBody->CastShadow = false;
	BuildingBody->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}

void AGeneralMapComponent::BeginPlay() {
	Super::BeginPlay();

	// overlap event
	if (IsValid(BuildingCollisionComp)) {
		BuildingCollisionComp->OnComponentBeginOverlap.AddUniqueDynamic(this,
			&AGeneralMapComponent::OnOverlapBegin);
		BuildingCollisionComp->OnComponentEndOverlap.AddUniqueDynamic(this,
			&AGeneralMapComponent::OnOverlapEnd);
	}
}

void AGeneralMapComponent::SetPlayerInArea(bool isInArea) {
	playerInArea = isInArea;
}

void AGeneralMapComponent::SetRandomRotation() {
	if (setRandomLocation) {
		float randomPitch = FMath::RandRange(-180, 180);
		float randomYaw = FMath::RandRange(-180, 180);
		float randomRoll = FMath::RandRange(-180, 180);

		FRotator RandomRotation = FRotator(randomPitch, randomYaw, randomRoll);
		SetActorRotation(RandomRotation);
	}
}

// is able to add some bonus for certain enemies in certain area
void AGeneralMapComponent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		APlayerClass* player = Cast<APlayerClass>(OtherActor);

		if (IsValid(player)) {
			PlayerInAreaResponse(player);
		}
	}
}

void AGeneralMapComponent::PlayerInAreaResponse(APlayerClass* player) {
	SetPlayerInArea(true);
	player->SetFloorType(floorType);
}

void AGeneralMapComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	return;
}

// no necesary
void AGeneralMapComponent::PlayerLeaveAreaResponse(APlayerClass* player) {
	SetPlayerInArea(false);
	player->SetFloorType(floorType);
}