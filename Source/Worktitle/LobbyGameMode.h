// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WORKTITLE_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

private:
	UPROPERTY()
	int NumberOfPlayers = 0;

	int TimerCount = 0.f;

	UFUNCTION()
	void StartGame();

	FTimerHandle ServerTravelHandler;
	
};
