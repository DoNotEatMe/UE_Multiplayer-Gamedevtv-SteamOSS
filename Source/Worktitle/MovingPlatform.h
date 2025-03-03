// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class WORKTITLE_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	float Speed = 20.f;

	UPROPERTY(EditAnywhere)	
	FVector Location;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;


	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;
	
	UPROPERTY(EditAnywhere)
	int ActiveTriggers = 0;

protected:
	

	virtual void BeginPlay() override;
};
