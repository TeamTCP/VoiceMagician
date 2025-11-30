// Fill out your copyright notice in the Description page of Project Settings.


#include "VMShopItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Materials/MaterialInterface.h" 
#include "Materials/MaterialInstanceDynamic.h"  
#include "VMShopScreen.h"
#include "VMShopItemTooltip.h"




UVMShopItemWidget::UVMShopItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UVMShopItemTooltip> TooltipRef(TEXT("/Game/Project/UI/Shop/WBP_VMShopItemToolTip.WBP_VMShopItemToolTip_C"));
	if (TooltipRef.Succeeded())
	{
		ItemTooltipClass = TooltipRef.Class;
	}
}

void UVMShopItemWidget::Setup(const FVMEquipmentInfo& Info)
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
		ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Info.AtlasCol);
		ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Info.AtlasRow);
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

	//툴팁 설정
	if (ItemTooltip != nullptr)
	{
		ItemTooltip->Setup(EquipmentInfo);
	}
}

void UVMShopItemWidget::NativeConstruct()
{
	if (ItemButton == nullptr)
	{
		return;
	}

	//중복 바인딩 방지. 그리드 Repack 시 또 다시 호출되기 때문에 막아줘야한다.
	if (!ItemButton->OnClicked.IsAlreadyBound(this, &UVMShopItemWidget::OnItemButtonClicked))
	{
		ItemButton->OnClicked.AddDynamic(this, &UVMShopItemWidget::OnItemButtonClicked);
	}

	//툴팁 위젯 생성
	if (ItemTooltipClass != nullptr)
	{
		ItemTooltip = CreateWidget<UVMShopItemTooltip>(GetWorld(), ItemTooltipClass);
		if (ItemTooltip != nullptr)
		{
			this->SetToolTip(ItemTooltip);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemTooltip is not create"));
		}
	}
}

void UVMShopItemWidget::OnItemButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Button Click"));
	if (ShopScreen != nullptr)
	{
		ShopScreen->OnGridItemButtonClicked(this); //위젯 자기자신 전달
	}
}
