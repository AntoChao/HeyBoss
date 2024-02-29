// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlienClass.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "../../../Weapons/MeleeWeaponClass.h"
#include "../../../Weapons/ProjectileWeaponClass.h"

#include "../../../Map/AlienPortalClass.h"

#include "GameFramework/Controller.h"

// navigation
#include "NavigationSystem.h"


AAlienClass::AAlienClass() {
	BarrierCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Coliision Comp"));

	BarrierCollisionComp->InitSphereRadius(1000.0f);
	BarrierCollisionComp->SetOnlyOwnerSee(false);
	BarrierCollisionComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BarrierCollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BarrierCollisionComp->CanCharacterStepUpOn = ECB_No;
	BarrierCollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	BarrierCollisionComp->SetupAttachment(GetMesh());
}

void AAlienClass::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	BarrierCollisionComp->OnComponentBeginOverlap.AddUniqueDynamic(this, 
		&AAlienClass::OnBarrierOverlapBegin);

	AttachBodyWeapons();
	StartIntroScene();
}

// only reflect 1 bullet
void AAlienClass::OnBarrierOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	
	if (IsBarrierActivated) {
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
			if (OtherActor->GetClass()->IsChildOf(AProjectileWeaponClass::StaticClass())) {
				AProjectileWeaponClass* bullet = Cast<AProjectileWeaponClass>(OtherActor);

				if (IsValid(bullet)) {
					ControlBullet(bullet);
					IsBarrierActivated = false;
				}
			}
		}
	}
}


// Intro Scene
void AAlienClass::StartIntroScene() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (IsValid(IntroUFOClass)) {
		GetWorld()->SpawnActor<AAlienUFOClass>(IntroUFOClass,
			GetActorLocation() + IntroSpawnOffSet, GetActorRotation() + IntroSpawnExtraRotation,
			ActorSpawnParams);
	}
	if (IsValid(IntroBloomClass)) {
		GetWorld()->SpawnActor<AAlienBloomClass>(IntroBloomClass,
			GetActorLocation() + IntroSpawnOffSet, GetActorRotation() + IntroSpawnExtraRotation, 
			ActorSpawnParams);
	}
}


void AAlienClass::AttachBodyWeapons() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(BodyWeaponClass)) {
		FTransform TransLeftHand = GetMesh()->
			GetSocketTransform(LHSocket, ERelativeTransformSpace::RTS_World);
		FTransform TransRightHand = GetMesh()->
			GetSocketTransform(RHSocket, ERelativeTransformSpace::RTS_World);
		FTransform TransFoot = GetMesh()->
			GetSocketTransform(FootSocket, ERelativeTransformSpace::RTS_World);

		LeftHand = GetWorld()->SpawnActor<AMeleeWeaponClass>(BodyWeaponClass,
			TransLeftHand, ActorSpawnParams);
		RightHand = GetWorld()->SpawnActor<AMeleeWeaponClass>(BodyWeaponClass,
			TransRightHand, ActorSpawnParams);
		Foot = GetWorld()->SpawnActor<AMeleeWeaponClass>(BodyWeaponClass,
			TransFoot, ActorSpawnParams);

		if (IsValid(LeftHand)) {
			LeftHand->SetWeaponController(this);
			LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, LHSocket);

			ActorsToIgnore.Add(LeftHand);
		}
		if (IsValid(RightHand)) {
			RightHand->SetWeaponController(this);
			RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, RHSocket);

			ActorsToIgnore.Add(RightHand);
		}
		if (IsValid(Foot)) {
			Foot->SetWeaponController(this);
			Foot->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, FootSocket);

			ActorsToIgnore.Add(Foot);
		}

		DefaultComponentQueryParams.AddIgnoredActors(ActorsToIgnore);
	}
}

void AAlienClass::ActivateWeapon() {
	LeftHand->Activate(FVector::ZeroVector);
	RightHand->Activate(FVector::ZeroVector);
	Foot->Activate(FVector::ZeroVector);
}

void AAlienClass::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bLookingAtPlayer) {
		LookAtPlayer();
	}
}

	// Override Actions
void AAlienClass::SelectActionToDo() {
	Super::SelectActionToDo();

	if (NeedToUnseenReposition()) {
		EnemyController->SetCurrentAction(EEnemyAction::EReposition);
	}
	else {
		float Distance = GetPlayerDistance();
		// int OriginalChance;
		if (Distance < MeleeRange) {
			// Melee Attack ->75% melee & 25% mid range
			if (GetRandomInRangeNum(100) < 75) {
				EnemyController->SetCurrentAction(EEnemyAction::EMeleeAction);
			}
			else {
				EnemyController->SetCurrentAction(EEnemyAction::EMidRangeAction);
			}
			
		}
		else if (Distance < MidRange) {
			// MidRange Attack -> 60 / 40
			if (GetRandomInRangeNum(100) < 60) {
				EnemyController->SetCurrentAction(EEnemyAction::EMidRangeAction);
			}
			else {
				EnemyController->SetCurrentAction(EEnemyAction::ERangedAction);
			}
			
		}
		else {
			// Ranged Attack -> 50 / 35 / 15 
			int Chance = GetRandomInRangeNum(100);
			if (Chance < 50) {
				EnemyController->SetCurrentAction(EEnemyAction::ERangedAction);
			}
			else if (Chance < 85){
				EnemyController->SetCurrentAction(EEnemyAction::EMidRangeAction);
			}
			else {
				EnemyController->SetCurrentAction(EEnemyAction::EMeleeAction);
			}
			
		}
	}
}

// Reposition
void AAlienClass::UnseenRepositionAction() {
	Super::UnseenRepositionAction();

	/* only tp*/
	PlaySelectedAnimMontage(TP_Montage, AnimMontagePlayRate);
}

FVector AAlienClass::GetTPBackwardLocation() {
	FVector MoveToVector = GetActorLocation() + GetPlayerSameHeightDirection() * -1 * RepositionTPDistance;

	return MoveToVector;
}
FVector AAlienClass::GetTPForwardLocation() {
	FVector MoveToVector = GetActorLocation() + GetPlayerSameHeightDirection() * RepositionTPDistance;
	return MoveToVector;
}

void AAlienClass::TeleportAroundPlayer() {
	FVector TPSpot;
	TPSpot = GetRandomLocation(GetPlayerLocation(), TeleportRadius);
	
	while (!LocationValidForTP(TPSpot)) {
		TPSpot = GetRandomLocation(GetPlayerLocation(), TeleportRadius);
	}

	FVector InitLocation = GetTPBackwardLocation();
	FVector PortalTargetLocation = TPSpot + FVector(0.0f, 0.0f, GetActorLocation().Z + PortalHeight);
	FVector PortalDirection = (PortalTargetLocation - Player->GetActorLocation()).GetSafeNormal();
	
	PortalTargetLocation = PortalTargetLocation + PortalDirection * PortalDist;

	SpawnPortal(InitLocation, true, PortalTargetLocation);
	SpawnPortal(PortalTargetLocation, false, InitLocation);
	
	SetActorLocation(TPSpot + FVector(0.0f, 0.0f, GetActorLocation().Z + 50.0f));
	
	LookAtPlayer();
}

void AAlienClass::SpawnPortal(FVector SpawnLocation, bool Activate, FVector TPTarget) {
	
	if (IsValid(PortalClass)) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FRotator PortalRotation = GetPlayerDirection().Rotation();

		AAlienPortalClass* ThePortal = GetWorld()->SpawnActor<AAlienPortalClass>(
			PortalClass, SpawnLocation, PortalRotation, ActorSpawnParams);

		if (IsValid(ThePortal)) {
			ThePortal->SetTeleport(this, Activate, TPTarget);
		}
	}
	/*
	// Play sound
	if (IsValid(PassPortalSoundCue)) {
		UGameplayStatics::PlaySoundAtLocation(this, PassPortalSoundCue,
			SpawnLocation);
	}

	// Play Niagara 
	
	if (IsValid(PortalNiagara)) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			PortalNiagara,
			SpawnLocation,
			GetPlayerDirection().Rotation(),
			FVector(1.0f, 1.0f, 1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
	}*/
}

// Melee
void AAlienClass::MeleeAction() {
	Super::MeleeAction();

	switch (GetRandomInRangeNum(MeleeActionNum)) {
		case 1: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Move BackwardReposition"));
			WrapperMoveToAttackOffset(GetBackwardRepositionLocation());

			break;
		}
		case 2: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Smash Ground"));

			SmashGround();
			break;
		}
		case 3: {
			float Distance = GetPlayerDistance();
			if (Distance < MeleeRange) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Kick"));

				Kick();
			}
			else {
				RegreatAction();
			}
			break;
		}
	}

}
	// Smash ground
void AAlienClass::SmashGround() {
	if (IsValid(LeftHand) && IsValid(RightHand)) {
		if (IsValid(SmashGround_Montage)) {
			PlaySelectedAnimMontage(SmashGround_Montage, AnimMontagePlayRate);
		}
	}
}

// MidRange Kick
void AAlienClass::Kick() {
	if (IsValid(Foot)) {
		if (IsValid(Kick_Montage)) {
			/* NEED TO SYNCRONYZE THE TIME -> ONLY ONE CHANGE DOINGTHING VARIABLE*/
			PlaySelectedAnimMontage(Kick_Montage, AnimMontagePlayRate);

			CustomMove(1.0f, GetPlayerFrontPosition());
		}
	}
}

// Mid Range
void AAlienClass::MidRangeAction() {
	Super::MidRangeAction();
	switch (GetRandomInRangeNum(MidRangeActionNum)) {
		case 1: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TP Punch"));

			TPPunch();
			break;
		}
		case 2: {
			
			if (IsBarrierActivated) {
				RegreatAction();
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Activate Barrier"));

				ActivateBarrier();
			}
			break;
		}
		case 3: {
			int left = GetRandomInRangeNum(2);
			if (left == 1) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Left Reposition"));
				WrapperMoveToAttackOffset(GetLeftRepositionLocation());
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Right Reposition"));
				WrapperMoveToAttackOffset(GetRightRepositionLocation());

			}
			break;
		}
	}
}

	// TP Punch
void AAlienClass::TPPunch() {
	if (IsValid(LeftHand) && IsValid(RightHand)) {

		if (IsValid(TPPunch_Montage)) {
			PlaySelectedAnimMontage(TPPunch_Montage, AnimMontagePlayRate);
		}
	}
}

	// Activate Barrier
void AAlienClass::ActivateBarrier() {
	if (IsValid(ActivateBarrier_Montage)) {
		PlaySelectedAnimMontage(ActivateBarrier_Montage, AnimMontagePlayRate);
	}
}
void AAlienClass::BarrierActivated() {
	IsBarrierActivated = true;
	SpawnAndAttachBarrier();
	GetWorldTimerManager().SetTimer(BarrierTimerHandle, this,
		&AAlienClass::DesactivateBarrier, BarrierActivateTime, true);
}

void AAlienClass::ControlBullet(AProjectileWeaponClass* ABullet) {
	ABullet->SetWeaponController(this);
	DesactivateBarrier();
	FVector FireDirection = (GetPlayerLocation() - ABullet->GetActorLocation()).GetSafeNormal();
	ABullet->SetProjectileDirection(FireDirection);
}

/*
void AAlienClass::DesactivateBarrier() {
	IsBarrierActivated = false;
}*/

/* Call in anim montage*/
void AAlienClass::TPFrontPlayer() {
	SetActorLocation(GetPlayerFrontPosition());
	LookAtPlayer();
}
// Range Action
void AAlienClass::RangedAction() {
	Super::RangedAction();

	switch (GetRandomInRangeNum(RangedActionNum)) {
		case 1: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("UFO Invation"));

			if (!IsSpawningUFO) {
				UFOInvation();
			}
			else {
				RegreatAction();
			}
			break;
		}
		case 2: {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Dragon Ball"));

			DragonBallLazer();
			break;
		}
	}
}

void AAlienClass::UFOInvation() {
	if (IsValid(SpawnUFOs_Montage)) {
		IsSpawningUFO = true;
		CurrentUFOAmount = 0;
		PlaySelectedAnimMontage(SpawnUFOs_Montage, AnimMontagePlayRate);
	}
}

void AAlienClass::SpawnUFOs() {
	if (CurrentUFOAmount < UFOAmount) {
		if (IsValid(UFOClass)) {
			FActorSpawnParameters ActorSpawnParams;

			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector SpawnLocation = GetRandomLocation(GetActorLocation(), UFOSpawnRadius);
			SpawnLocation.Z = UFOSpawnHeight;

			GetWorld()->SpawnActor<AAlienUFOClass>(UFOClass, SpawnLocation,
				FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);
			
			CurrentUFOAmount++;

			GetWorldTimerManager().SetTimer(SpawnUFOTimer, this,
				&AAlienClass::SpawnUFOs, UFOSpawnCooldown, false);
		}
	}
	else {
		IsSpawningUFO = false;
	}
}

void AAlienClass::DragonBallLazer() {
	if (IsValid(DragonBallLazer_Montage)) {
		PlaySelectedAnimMontage(DragonBallLazer_Montage, AnimMontagePlayRate);
	}
}

void AAlienClass::PrepareDBball() {
	DBBall = nullptr;
	if (IsValid(DBProjectileClass)) {
		FActorSpawnParameters ActorSpawnParams;
		// ActorSpawnParams.Owner = this;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform TransHandDB = GetMesh()->
			GetSocketTransform(RHDBSocket, ERelativeTransformSpace::RTS_World);

		DBBall = GetWorld()->SpawnActor<AProjectileWeaponClass>(DBProjectileClass,
			TransHandDB, ActorSpawnParams);

		if (IsValid(DBBall)) {
			DBBall->SetWeaponController(this);
			DBBall->SetProjectileIgnoreOwner();
			IgnoreDBBall(DBBall);
			/*
			DBBall->AttachToComponent(GetMesh(), FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, RHDBSocket);*/
		}
	}
}

void AAlienClass::LaunchDBball() {
	if (IsValid(DBBall)) {
		// Set Bullet Velocity
		// DBBall->GetRootComponent()->DetachFromParent(true);

		FVector FireDirection = (GetPlayerLocation() - DBBall->GetActorLocation()).GetSafeNormal();
		DBBall->SetProjectileDirection(FireDirection);
	}
}

void AAlienClass::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	Super::OnMontageEnded(Montage, bInterrupted);
	
	LeftHand->Desactivate();
	RightHand->Desactivate();
	Foot->Desactivate();
}