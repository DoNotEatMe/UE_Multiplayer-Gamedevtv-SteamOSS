// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class WORKTITLE_API UGameMenu : public UMenuWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* Cancel;


	UFUNCTION()
	void BackToMainMenu();

};
