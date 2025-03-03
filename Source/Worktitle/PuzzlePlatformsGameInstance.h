// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"


class IOnlineSubsystem;
class FOnlineSearchSettings;
class UMenuWidget;
class UGameMenu;
class UMainMenu;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};
/**
 * 
 */
UCLASS()
class WORKTITLE_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;

	UFUNCTION(Exec)
	virtual void Host(FString SessionName) override;

	UFUNCTION(Exec)
	virtual void Join(const uint32 Index) override;
	
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void RefreshServerList() override;
	
	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();
	
private:
	TSubclassOf<UUserWidget> MenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	UPROPERTY()
	UMainMenu* Menu;
	
	UPROPERTY()
	UMenuWidget* GameMenu;

	UFUNCTION()
	void LoadMainMenu() override;

	IOnlineSessionPtr SessionInterface;

	void SetSessionInterface(IOnlineSubsystem* Subsystem);
	
	void OnCreateSessionComplete(FName SessionName, bool Succeeded);

	void OnDestroySessionComplete(FName SessionName, bool Succeeded);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	FString DesiredServerName;
	void CreateSession(FString ServerName);
	
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void FindSessions();

	void OnFindSessionsComplete(bool Succeeded);

	

	
	
	
};
