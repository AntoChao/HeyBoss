// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../World/Common_Enums.h"
#include "../Characters/Player/PlayerClass.h"

#include "GeneralMapComponent.generated.h"

UCLASS()
class AGeneralMapComponent : public AActor
{
	GENERATED_BODY()

public:
	AGeneralMapComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		class USceneComponent* BuildingSceneComponent;

	/* Mesh*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		class UStaticMeshComponent* BuildingBody;

	/* Collision Comp*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		class UBoxComponent* BuildingCollisionComp;

	UFUNCTION(BlueprintCallable, Category = "Building Area overlap")
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = "Building Area overlap")
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// consequences when player get in to the area
	UFUNCTION(BlueprintCallable, Category = "Building Area overlap")
		virtual void PlayerInAreaResponse(APlayerClass* player);
	UFUNCTION(BlueprintCallable, Category = "Building Area overlap")
		virtual void PlayerLeaveAreaResponse(APlayerClass* player);

	// Check if player is in building area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool playerInArea = false;

	UFUNCTION(BlueprintCallable, Category = "Player")
		void SetPlayerInArea(bool isInArea);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area")
		EFloorType floorType = EFloorType::EConcrete;

	// Random Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Rotation")
		bool setRandomLocation = false; /*a variable to be changed in blueprint*/
	UFUNCTION(BlueprintCallable, Category = "Random Rotation")
		void SetRandomRotation();

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};



