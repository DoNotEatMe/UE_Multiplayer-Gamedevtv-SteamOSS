// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class WORKTITLE_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentPlayers;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxPlayers;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;
	
	void Setup(class UMainMenu* Parent, uint32 Index);

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	uint32 Index;

	UPROPERTY()
	UMainMenu* Parent;

	UFUNCTION()
	void OnClicked();

};
