// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/VMShopScreen.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/ListView.h"
#include "Game/VMRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Shop/VMShopItemWidget.h"
#include "UI/Shop/VMShopItemDataObject.h"
#include "Item/Equipment/VMEquipmentInfo.h"

UVMShopScreen::UVMShopScreen(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UVMShopItemWidget> ShopItemWidgetRef (TEXT("/Game/Project/UI/Shop/WBP_ShopItemWidget.WBP_ShopItemWidget_C"));
    if (ShopItemWidgetRef.Succeeded())
    {
        ShopItemWidgetClass = ShopItemWidgetRef.Class;
    }
}

void UVMShopScreen::NativeConstruct()
{


    //버튼 함수 바인딩
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

void UVMShopScreen::SetShop(const TArray<FVMEquipmentInfo>& ShopItems)
{
    UE_LOG(LogTemp, Log, TEXT("Open Shop!!"));

    if (ShopGridPanel == nullptr)
    {
        return;
    }
    if (ShopItemWidgetClass == nullptr)
    {
        return;
    }

    ShopGridPanel->ClearChildren(); //그리드 패널 재설정

    int32 Index = 0;
    for (const FVMEquipmentInfo &Item : ShopItems)
    {
        UE_LOG(LogTemp, Log, TEXT("Item : %s"), *Item.ItemName);
        UVMShopItemWidget* NewItemWidget = CreateWidget<UVMShopItemWidget>(this, ShopItemWidgetClass);

        NewItemWidget->SetUp(Item);
        NewItemWidget->ShopScreen = this;
        int32 Row = Index / 5;
        int32 Col = Index % 5;
        ShopGridPanel->AddChildToUniformGrid(NewItemWidget, Row, Col);
        Index++;
    }

}

void UVMShopScreen::OnGridItemButtonClicked(const FVMEquipmentInfo &ClickedItemInfo)
{
    UE_LOG(LogTemp, Log, TEXT("Clicked: %s"), *ClickedItemInfo.ItemName);
    
    TArray<UObject*> Items = ShopListView->GetListItems();
    bool bIsFind = false;

    // 아이템이 이미 리스트에 추가되어있는지 확인
    for (UObject* ItemObject : Items)
    {
        UVMShopItemDataObject* ItemData = Cast<UVMShopItemDataObject>(ItemObject);
        if (ItemData != nullptr)
        {
            if (ItemData->EquipmentInfo->ItemID == ClickedItemInfo.ItemID)
            {
                bIsFind = true;
                //추가 되어있다면 새로 만들지말고 기존 아이템 Count 증가
                ItemData->ChangeItemCount(true); 
                break;
            }
        }
    }

    //추가되어 있지않다며 새로 아이템 추가
    if (!bIsFind)
    {
        UVMShopItemDataObject* NewShopItemDataObject = NewObject<UVMShopItemDataObject>(this);
        NewShopItemDataObject->EquipmentInfo = &ClickedItemInfo;
        ShopListView->AddItem(NewShopItemDataObject);
    }

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
