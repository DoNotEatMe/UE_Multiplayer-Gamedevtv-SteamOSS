// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"

#include "Components/Button.h"

bool UGameMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;;
	
	MainMenu->OnClicked.AddDynamic(this,&UGameMenu::BackToMainMenu);
	Cancel->OnClicked.AddDynamic(this,&UGameMenu::TearDown);

	return true;
}

void UGameMenu::BackToMainMenu()
{
	this->TearDown();
	if (MenuInterface != nullptr)
	{
		MenuInterface->LoadMainMenu();
	}
}

