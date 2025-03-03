// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Blueprint/UserWidget.h"

#include "MainMenu.generated.h"

struct FServerData;
class UEditableTextBox;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class WORKTITLE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu();

	void SetServerList(TArray<FServerData> ServerData);
	
	virtual void SetMenuInterface(IMenuInterface* NewMenuInterface);
	virtual void Setup();

	void SelectIndex(int32 Index);

private:
	UPROPERTY(meta=(BindWidget))
	UButton* Host;

	UPROPERTY(meta=(BindWidget))
	UButton* Switch;

	UPROPERTY(meta=(BindWidget))
	UButton* Join;

	UPROPERTY(meta=(BindWidget))
	UButton* Cancel;

	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta=(BindWidget))
	UButton* HostButton;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* Switcher;

	UPROPERTY(meta=(BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta=(BindWidget))
	UWidget* MainMenu;
	
	UPROPERTY(meta=(BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta=(BindWidget))
	UPanelWidget* ServerList;

	UPROPERTY()
	TSubclassOf<UUserWidget> ServerRowClass;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* ServerHostName;


	UFUNCTION()
	void HandleHostServerButtonClicked();
	
	UFUNCTION()
	void HandleHostButtonClicked();

	UFUNCTION()
	void HandleJoinButtonClicked();

	UFUNCTION()
	void HandleCancelButtonClicked();

	UFUNCTION()
	void HandleSwitchJoinButtonClicked();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();	

	


protected:
	virtual bool Initialize() override;
};
