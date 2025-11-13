// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/VMShopScreen.h"
#include "Components/Button.h"
#include "Game/VMRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Hero/VMCharacterHeroBase.h"

void UVMShopScreen::NativeConstruct()
{
    if (BuyButton != nullptr)
    {
        BuyButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnBuyClicked);
    }

    if (SellButton != nullptr)
    {
        SellButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnSellClicked);
    }

    if (ExitButton != nullptr)
    {
        ExitButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnExitClicked);
    }

    UpdateButtonStyle();
}

void UVMShopScreen::OnBuyClicked()
{
    if (bIsBuy)
    {
        return;
    }

    bIsBuy = true;
    UpdateButtonStyle();
}

void UVMShopScreen::OnSellClicked()
{
    if (!bIsBuy)
    {
        return;
    }

    bIsBuy = false;
    UpdateButtonStyle();
}

void UVMShopScreen::OnExitClicked()
{
    ////다음에 들어올 때 구입부터 나오도록 bool 설정
    bIsBuy = true;
    AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PC == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("PlayerController is not found"));
        return;
    }
    PC->ShowScreen(EScreenUIType::GameScreen);
    PC->bShowMouseCursor = false;
    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter != nullptr)
    {
        AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(PlayerCharacter);
        if (Player != nullptr)
        {
            Player->ChangeInputMode(EInputMode::Default);
        }
    }
}

void UVMShopScreen::UpdateButtonStyle()
{
    FLinearColor SelectColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#072C7FCC"))); //파란색
    FLinearColor UnselectColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#ADBBB780")));//회색

    if (BuyButton!=nullptr)
    {
        BuyButton->SetBackgroundColor(bIsBuy ? SelectColor : UnselectColor);
    }

    if (SellButton!=nullptr)
    {
        SellButton->SetBackgroundColor(bIsBuy ? UnselectColor : SelectColor);
    }
}
