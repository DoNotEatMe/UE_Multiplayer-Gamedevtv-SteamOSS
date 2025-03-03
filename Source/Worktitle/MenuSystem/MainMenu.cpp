// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ServerRow.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Worktitle/PuzzlePlatformsGameInstance.h"

UMainMenu::UMainMenu()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Udemy/UI/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;;

	Host->OnClicked.AddDynamic(this, &UMainMenu::HandleHostServerButtonClicked);
	Switch->OnClicked.AddDynamic(this, &UMainMenu::HandleSwitchJoinButtonClicked);

	Cancel->OnClicked.AddDynamic(this, &UMainMenu::HandleCancelButtonClicked);
	Join->OnClicked.AddDynamic(this, &UMainMenu::HandleJoinButtonClicked);

	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::HandleCancelButtonClicked);
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HandleHostButtonClicked);

	return true;
}


void UMainMenu::HandleHostButtonClicked()
{
	if (MenuInterface != nullptr)
	{
		if (FString Name = ServerHostName->GetText().ToString(); !Name.IsEmpty())
		{
			MenuInterface->Host(Name);	
		} else
		{
			return;
		}
		
	}
}


void UMainMenu::SetServerList(TArray<FServerData> ServerData)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& Server : ServerData)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(Server.Name));
		Row->HostUser->SetText(FText::FromString(Server.HostUserName));
		Row->CurrentPlayers->SetText(FText::FromString(FString::Printf(TEXT("%i"),Server.CurrentPlayers)));
		Row->MaxPlayers->SetText(FText::FromString(FString::Printf(TEXT("%i"),Server.MaxPlayers)));
		Row->Setup(this, i);
		++i;
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(int32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		if (UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i)))
		{
			Row->Selected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i; 
		}
	}
}

void UMainMenu::HandleJoinButtonClicked()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex not set"));
	}
}


void UMainMenu::HandleCancelButtonClicked()
{
	if (!ensure(MainMenu != nullptr)) return;
	Switcher->SetActiveWidget(MainMenu);
}

void UMainMenu::HandleHostServerButtonClicked()
{
	if (!ensure(MainMenu != nullptr)) return;
	Switcher->SetActiveWidget(HostMenu);
}

void UMainMenu::HandleSwitchJoinButtonClicked()
{
	if (!ensure(JoinMenu != nullptr)) return;
	Switcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
	Super::SetMenuInterface(NewMenuInterface);
}

void UMainMenu::Setup()
{
	Super::Setup();
}
