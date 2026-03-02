// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/VMInventoryPanel.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "UI/Inventory/VMEquipmentPanel.h"
#include "Inventory/VMInventoryComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Character/VMCharacterHeroHUD.h"
#include "Hero/VMHeroStatComponent.h"

#include "Components/WrapBox.h"
#include "Components/TextBlock.h"

void UVMInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Character = Cast<AVMCharacterHeroBase>(GetOwningPlayerPawn());

    if (Character)
    {
        InventoryReference = Character->GetInventory();
        if (InventoryReference)
        {
            InventoryReference->OnInventoryUpdated.AddUObject(this, &UVMInventoryPanel::RefreshInventory);
            SetInfoText();
        }
    }
}

void UVMInventoryPanel::RefreshInventory()
{
    const TArray<UVMEquipment*>& Contents = InventoryReference->GetInventoryContents();

    InventoryWrapBox->ClearChildren();

    for (UVMEquipment* const& InventoryItem : Contents)
    {
        UVMInventoryItemSlot* ItemSlot = CreateWidget<UVMInventoryItemSlot>(this, InventorySlotClass);

        ItemSlot->SlotType = ESlotType::Inventory;
        ItemSlot->InventoryPanelRef = this;

        ItemSlot->SetItemReference(InventoryItem);  // 여기서 SetItemReference 로그가 떠야 함

        InventoryWrapBox->AddChildToWrapBox(ItemSlot);
    }

    SetInfoText();
}

void UVMInventoryPanel::HandleItemDoubleClicked(UVMEquipment* Item)
{
    if (!Item || !InventoryReference) return;

    AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(InventoryReference->GetOwner());
    if (!Hero) return;

    // 1) 스탯 적용
    Hero->GetStatComponent()->ApplyEquipmentStats(Item);

    // 2) 장비 패널 UI
    if (LinkedEquipmentPanel)
    {
        const int32 EquipIndex = LinkedEquipmentPanel->TryEquipToEmptySlot(Item);

        if (EquipIndex != INDEX_NONE)
        {
            InventoryReference->RemoveSingleInstanceOfItem(Item);
            RefreshInventory();
        }
    }
}   

void UVMInventoryPanel::SetInfoText() const
{
}

bool UVMInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
