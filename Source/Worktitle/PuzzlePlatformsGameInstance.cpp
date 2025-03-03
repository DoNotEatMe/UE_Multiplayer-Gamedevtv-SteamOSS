// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "MenuSystem/MainMenu.h"


const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
/**
 * Initializes a new instance of the UPuzzlePlatformsGameInstance class.
 * This constructor searches for and loads the main menu widget class.
 *
 * @param ObjectInitializer An optional object initializer that can customize how objects are constructed.
 * @return A newly constructed instance of the UPuzzlePlatformsGameInstance class.
 */
UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//UE_LOG(LogTemp,Warning,TEXT("Game Instance constructor"));
	ConstructorHelpers::FClassFinder<UUserWidget> MenuFind(TEXT("/Game/Udemy/UI/WBP_MainMenu"));
	if (!ensure(MenuFind.Class != nullptr)) return;
	MenuClass = MenuFind.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuFind(TEXT("/Game/Udemy/UI/WBP_GameMenu"));
	if (!ensure(InGameMenuFind.Class != nullptr)) return;
	InGameMenuClass = InGameMenuFind.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());


	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld(), "STEAM");
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
	}
	else
	{
		Subsystem = Online::GetSubsystem(GetWorld(), "NULL");
		UE_LOG(LogTemp, Warning, TEXT("Subsystem not found"));
		UE_LOG(LogTemp, Warning, TEXT("Now Subsystem is: %s "), *Subsystem->GetSubsystemName().ToString());
	}

	SetSessionInterface(Subsystem);
}

void UPuzzlePlatformsGameInstance::SetSessionInterface(IOnlineSubsystem* Subsystem)
{
	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found SessionInterface "));
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);

		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
	}
}

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	if (SessionInterface.IsValid())
	{
		if (SessionInterface->GetNamedSession(SESSION_NAME) != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			DesiredServerName = ServerName;
			CreateSession(DesiredServerName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid"));
	}
}


void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (!Succeeded)
	{
		return;
	}

	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, (TEXT("Hosting %s"), *SessionName.ToString()));
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Udemy/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		CreateSession(DesiredServerName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, (TEXT("Destroying %s failed"), *SessionName.ToString()));
	}
}


void UPuzzlePlatformsGameInstance::CreateSession(FString ServerName)
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		SessionSettings.bIsLANMatch = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";

		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		if (!DesiredServerName.IsEmpty())
		{
			SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, ServerName,
			                    EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	UE_LOG(LogTemp, Warning, TEXT("Session refreshing start "));
	FindSessions();
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Session Complete"));

	if (!Succeeded || !SessionSearch.IsValid())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Session count: %d"), SessionSearch->SearchResults.Num());

	TArray<FServerData> Servernames;
	for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Name found: %s"), *SearchResult.GetSessionIdStr());
		FServerData Data;

		FString SessionName;
		if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, SessionName))
		{
			Data.Name = SessionName;
		}
		else
		{
			Data.Name = "Couldn't find name";
		}


		Data.HostUserName = SearchResult.Session.OwningUserName;
		Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;


		Servernames.Add(Data);
	}

	Menu->SetServerList(Servernames);
}


void UPuzzlePlatformsGameInstance::Join(const uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}


void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (!SessionInterface.IsValid()) return;

		FString Address;
		if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
		{
			UE_LOG(LogTemp, Error, TEXT("Could not get connect string."));
		}


		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Joining")));

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr)) return;

		//"192.168.1.67:7777" - editor
		//"127.0.0.1:7777" - standalone
		PlayerController->ClientTravel(Address, TRAVEL_Absolute);

		if (PlayerController->GetNetMode() == NM_Standalone)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
			                                 TEXT(
				                                 "Failed to join the session. Please check the IP Address or Network connection."));
			return;
		}
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;


	Menu->Setup();
	Menu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	GameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(GameMenu != nullptr)) return;
	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Udemy/Maps/Menu", TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::FindSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 500;
		SessionSearch->bIsLanQuery = false;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}
