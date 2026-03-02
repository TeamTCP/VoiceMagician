// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMCombineSlot.h"
#include "Inventory/VMItemDragDropOperation.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "Item/Equipment/VMEquipment.h"
#include "Components/Border.h"
#include "Components/Image.h"


bool UVMCombineSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UE_LOG(LogTemp, Warning, TEXT("=== UVMCombineSlot::NativeOnDrop ENTER ==="));

    UVMItemDragDropOperation* DragOp = Cast<UVMItemDragDropOperation>(InOperation);
    if (!DragOp || !DragOp->Payload)
    {
        UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: DragOp or Payload is NULL"));
        return false;
    }

    UVMEquipment* DroppedItem = Cast<UVMEquipment>(DragOp->Payload);
    if (!DroppedItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: Payload is not UVMEquipment"));
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: DroppedItem = %s"), *DroppedItem->GetName());

    StoredItem = DroppedItem;
    RefreshFromStoredItem();   // ★ 여기서 실제로 아이콘 표시

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(PC->GetPawn()))
        {
            if (UVMInventoryComponent* Inv = Hero->GetInventory())
            {
                Inv->RemoveEquipmentFromInventory(DroppedItem);
                UE_LOG(LogTemp, Warning, TEXT("CombineSlot::NativeOnDrop: Removed item from inventory"));
            }
        }
    }

    return true;

    if (DropBorder)
    {
        DropBorder->SetBrushColor(FLinearColor::Yellow);
    }

    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: StoredItem SET OK"));
    return true;
}

void UVMCombineSlot::RefreshFromStoredItem()
{
    if (!InnerItemSlot)
    {
        UE_LOG(LogTemp, Warning, TEXT("RefreshFromStoredItem: InnerItemSlot is NULL"));
        return;
    }

    // 인벤토리 슬롯에서 이미 쓰는 SetItemReference 함수 재사용
    InnerItemSlot->SetItemReference(StoredItem);
}