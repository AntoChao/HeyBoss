// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../World/Common_Enums.h"
#include "../Characters/Player/PlayerClass.h"
#include "GeneralMapComponent.h"

#include "AlienPortalClass.generated.h"

UCLASS()
class AAlienPortalClass : public AGeneralMapComponent
{
	GENERATED_BODY()

public:
	AAlienPortalClass();

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// consequences when player get in to the area
	virtual void PlayerInAreaResponse(APlayerClass* player) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Area overlap")
		bool PortalIsActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Area overlap")
		class ACharacterClass* PortalOwner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Area overlap")
		FVector TeleportLocation = FVector(0.0f, 0.0f, 0.0f);

public: 
	UFUNCTION(BlueprintCallable, Category = "Building Area overlap")
		void SetTeleport(ACharacterClass* ItsOwner, bool Activation, 
			FVector ATPLocation);

};



