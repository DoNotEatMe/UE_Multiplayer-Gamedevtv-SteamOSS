// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (NewPlayer)
	{
		
		UE_LOG(LogTemp,Warning,TEXT("Player loggined : %s"),*NewPlayer->GetName());
		NumberOfPlayers++;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("ALobbyGameMode::PostLogin PlayersCount: %d"),NumberOfPlayers));


		if (NumberOfPlayers >= 2)
		{
			GetWorldTimerManager().SetTimer(ServerTravelHandler, this, &ALobbyGameMode::StartGame, 10, false);
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	NumberOfPlayers--;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("ALobbyGameMode::Logout PlayersCount: %d"),
	                                 NumberOfPlayers));
}

void ALobbyGameMode::StartGame()
{

	
	if (auto Instance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance()); Instance)
	{
		Instance->StartSession();
	}
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel(
		  "/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	
}
