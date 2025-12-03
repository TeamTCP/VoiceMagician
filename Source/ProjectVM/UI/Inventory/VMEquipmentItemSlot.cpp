// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMEquipmentItemSlot.h"

#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/Equipment/VMEquipment.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"


void UVMEquipmentItemSlot::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Warning,
        TEXT("EquipmentItemSlot::NativeConstruct %s, AtlasMaterial=%s"),
        *GetName(),
        *GetNameSafe(AtlasMaterial));

    if (ItemBorder)
    {
        ItemBorder->SetBrushColor(FLinearColor::Transparent);
    }

    // Atlas 머터리얼에서 MID 생성
    if (ItemIcon && AtlasMaterial)
    {
        ItemMaterialInstance = UMaterialInstanceDynamic::Create(AtlasMaterial, this);
        ItemIcon->SetBrushFromMaterial(ItemMaterialInstance);
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EquipmentItemSlot::NativeConstruct AtlasMaterial or ItemIcon is NULL"));
    }

    // 시작은 숨김
    if (ItemIcon)  ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    if (ItemBorder) ItemBorder->SetVisibility(ESlateVisibility::Hidden);

}

void UVMEquipmentItemSlot::SetItem(UVMEquipment* InItem)
{
    ItemReference = InItem;
    UE_LOG(LogTemp, Warning,
        TEXT("InventorySlot(%s)::SetItemReference Item=%s"),
        *GetName(),
        *GetNameSafe(ItemReference));
    RefreshFromItem();
}

void UVMEquipmentItemSlot::ClearItem()
{
    ItemReference = nullptr;
    SetItem(nullptr);
}

void UVMEquipmentItemSlot::RefreshFromItem()
{
 
    UE_LOG(LogTemp, Warning,
        TEXT("InventorySlot::RefreshFromItem ENTER, Ref=%s"),
        *GetNameSafe(ItemReference));

    if (!ItemIcon || !ItemBorder)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("RefreshFromItem: ItemIcon or ItemBorder is NULL"));
        return;
    }

    // ★ 슬롯이 비어 있을 때 처리
    if (!ItemReference)
    {
        ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Warning,
            TEXT("RefreshFromItem: Empty slot -> hide icon"));
        return;
    }

    ItemIcon->SetVisibility(ESlateVisibility::Visible);

    // ★ 브러시에서 머티리얼/텍스처 가져오기
    UObject* ResObj = ItemIcon->GetBrush().GetResourceObject();
    UMaterialInterface* BaseMat = Cast<UMaterialInterface>(ResObj);

    UE_LOG(LogTemp, Warning,
        TEXT("RefreshFromItem: ItemIcon=%p, ResObj=%p, BaseMat=%p, MID=%p"),
        ItemIcon.Get(),
        ResObj,
        BaseMat,
        ItemMaterialInstance.Get());

    if (!BaseMat)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("RefreshFromItem: BaseMat is NULL. "
                "Check ItemIcon Brush uses a material (e.g. M_ItemAtlas)."));
        return;
    }

    // ★ MID 없으면 여기서 무조건 생성
    if (!ItemMaterialInstance)
    {
        ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
        ItemIcon->SetBrushFromMaterial(ItemMaterialInstance);

        UE_LOG(LogTemp, Warning,
            TEXT("RefreshFromItem: MID created=%p"), ItemMaterialInstance.Get());
    }

    if (!ItemMaterialInstance)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("RefreshFromItem: ItemMaterialInstance STILL NULL"));
        return;
    }

    // ★ 여기서 아이템의 Atlas 정보 적용
    //    (구조체/함수 이름은 네 프로젝트에 맞게 수정)
    const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

    ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Info.AtlasCol);
    ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Info.AtlasRow);

    UE_LOG(LogTemp, Warning,
        TEXT("RefreshFromItem: Set Atlas Col=%d Row=%d"),
        Info.AtlasCol, Info.AtlasRow);
}
