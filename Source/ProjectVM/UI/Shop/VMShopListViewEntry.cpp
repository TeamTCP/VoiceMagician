// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/VMShopListViewEntry.h"
#include "UI/Shop/VMShopItemDataObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInterface.h" 
#include "Materials/MaterialInstanceDynamic.h"  

void UVMShopListViewEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	ShopItemDataObject = Cast<UVMShopItemDataObject>(ListItemObject);

	if (ShopItemDataObject == nullptr)
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
		ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), 1);
		ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), 1);
	}

	//아이템 변경사항 구독
	ShopItemDataObject->OnItemCountUpdated.AddUObject(this, &UVMShopListViewEntry::ChangeItemCount);

	ChangeItemCount();
}

void UVMShopListViewEntry::ChangeItemCount()
{
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