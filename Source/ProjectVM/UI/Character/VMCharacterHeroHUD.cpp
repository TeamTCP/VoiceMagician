// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/VMCharacterHeroHUD.h"
#include "UI/Inventory/VMInteractionWidget.h"
#include "UI/Inventory/VMMainMenu.h"

AVMCharacterHeroHUD::AVMCharacterHeroHUD()
{

}

void AVMCharacterHeroHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UVMMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UVMInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}


void AVMCharacterHeroHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AVMCharacterHeroHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void AVMCharacterHeroHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AVMCharacterHeroHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AVMCharacterHeroHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AVMCharacterHeroHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}