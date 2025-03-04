// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	Location = GetActorLocation();

	if (HasAuthority()){
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (HasAuthority() && ActiveTriggers > 0)
	{
		Location = GetActorLocation();
		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTraveled = (Location - GlobalStartLocation).Size();
		if (JourneyTraveled > JourneyLength)
		{
			FVector Swap = GlobalStartLocation;
			GlobalStartLocation = Location;
			GlobalTargetLocation = Swap;
		}
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		Location += Speed * DeltaSeconds * Direction;
		SetActorLocation(Location);
	}
}




void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	ActiveTriggers--;
	int Clamp = FMath::Clamp(ActiveTriggers,0,INT_MAX);
	ActiveTriggers = Clamp;
	
}