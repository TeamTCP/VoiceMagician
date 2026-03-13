// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/VMShopListViewEntry.h"
#include "UI/Shop/VMShopItemDataObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Materials/MaterialInterface.h" 
#include "Materials/MaterialInstanceDynamic.h"  
#include "Components/ListView.h"
#include "VMShopItemTooltip.h"

UVMShopListViewEntry::UVMShopListViewEntry(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UVMShopItemTooltip> TooltipRef(TEXT("/Game/Project/UI/Shop/WBP_VMShopItemToolTip.WBP_VMShopItemToolTip_C"));
	if (TooltipRef.Succeeded())
	{
		ItemTooltipClass = TooltipRef.Class;
	}
}

void UVMShopListViewEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	ShopItemDataObject = Cast<UVMShopItemDataObject>(ListItemObject);

	if (ShopItemDataObject == nullptr)
	{
		return;
	}

	if (ShopItemDataObject->EquipmentInfo == nullptr)
	{
		return;
	}

	if (ItemNameText != nullptr)
	{
		ItemNameText->SetText(FText::FromString(ShopItemDataObject->EquipmentInfo->ItemName));

	}

	if (ItemImage)
	{
		UMaterialInterface* BaseMat = Cast<UMaterialInterface>(ItemImage->GetBrush().GetResourceObject());
		if (BaseMat)
		{
			// Dynamic Material이면 Parent를 원본 Material로 변경
			if (UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(BaseMat))
			{
				BaseMat = DynMat->GetBaseMaterial(); // 원본 Material로 가져오기
			}

			ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, GetTransientPackage());
			ItemImage->SetBrushFromMaterial(ItemMaterialInstance);
		}
	}

	if (ItemMaterialInstance)
	{
		ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), ShopItemDataObject->EquipmentInfo->AtlasCol);
		ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), ShopItemDataObject->EquipmentInfo->AtlasRow);
	}

	//툴팁 설정
	if (ItemTooltip != nullptr)
	{
		ItemTooltip->Setup(ShopItemDataObject->EquipmentInfo);
	}

	//아이템 변경사항 구독
	ShopItemDataObject->OnItemCountUpdated.AddUObject(this, &UVMShopListViewEntry::ChangeItemCount);

	ChangeItemCount();

}

void UVMShopListViewEntry::NativeConstruct()
{
	if (ItemButton != nullptr)
	{
		ItemButton->OnClicked.AddDynamic(this, &UVMShopListViewEntry::OnListButtonClicked);
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

void UVMShopListViewEntry::ChangeItemCount()
{
	if (ShopItemDataObject->CurrentCount <= 0)
	{
		UListView* List = Cast<UListView>(GetOwningListView());
		if (List)
		{
			List->RemoveItem(ShopItemDataObject);
		}
	}

	if (ItemCountText != nullptr)
	{
		ItemCountText->SetText(FText::AsNumber(ShopItemDataObject->CurrentCount));
	}
	if (ItemAllPriceText != nullptr)
	{
		int NowMoney = ShopItemDataObject->CurrentCount * ShopItemDataObject->EquipmentInfo->ItemLevel * 2000;
		ItemAllPriceText->SetText(FText::AsNumber(NowMoney));
	}
}

void UVMShopListViewEntry::OnListButtonClicked()
{
	if (ShopItemDataObject == nullptr)
	{	
		UE_LOG(LogTemp, Warning, TEXT("ShopItemDataObject is nullptr"));
		return;
	}
	ShopItemDataObject->ChangeItemCount(false);
}