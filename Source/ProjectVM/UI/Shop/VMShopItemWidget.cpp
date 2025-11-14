// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/VMShopItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Materials/MaterialInterface.h" 
#include "Materials/MaterialInstanceDynamic.h"  
#include "UI/Shop/VMShopScreen.h"

void UVMShopItemWidget::SetUp(const FVMEquipmentInfo& Info)
{
	// 다이나믹 머터리얼 생성
	if (ItemImage && ItemImage->GetBrush().GetResourceObject())
	{
		UMaterialInterface* BaseMat = Cast<UMaterialInterface>(ItemImage->GetBrush().GetResourceObject());
		if (BaseMat != nullptr)
		{
			ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
			ItemImage->SetBrushFromMaterial(ItemMaterialInstance);
		}
	}
	//머터리얼 파라미터 설정
	if (ItemMaterialInstance)
	{
		ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), 1);
		ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), 1);
	}

	EquipmentInfo = &Info; //복사 없이 주소만 저장한다.

	if (ItemNameText != nullptr)
	{
		ItemNameText->SetText(FText::FromString(EquipmentInfo->ItemName));
	}
	if (ItemPriceText != nullptr)
	{
		ItemPriceText->SetText(FText::AsNumber(EquipmentInfo->ItemLevel * 2000));
	}
}

void UVMShopItemWidget::NativeConstruct()
{
	if (ItemButton != nullptr)
	{
		ItemButton->OnClicked.AddDynamic(this, &UVMShopItemWidget::OnItemButtonClicked);
	}
}

void UVMShopItemWidget::OnItemButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Button Click"));
	if (ShopScreen != nullptr)
	{
		ShopScreen->OnGridItemButtonClicked(*EquipmentInfo); //포인터의 구조체 전달
	}

}
