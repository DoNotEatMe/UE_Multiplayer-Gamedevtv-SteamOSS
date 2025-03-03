// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MainMenu.h"
#include "VisualizeTexture.h"
#include "Components/Button.h"

void UServerRow::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	Parent = InParent;
	Index = InIndex;
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);	
}
