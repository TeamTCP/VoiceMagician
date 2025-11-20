// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMEquipmentPanel.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "Item/Equipment/VMEquipment.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Character/VMCharacterHeroHUD.h"

#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"


void UVMEquipmentPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // 필요하면 시작 시 바로 슬롯 만들어두기
    EnsureSlotsInitialized();
}

void UVMEquipmentPanel::EnsureSlotsInitialized()
{
    // 이미 만들어져 있으면 다시 만들 필요 없음
    if (WeaponSlots.Num() > 0)
        return;

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel::EnsureSlotsInitialized CALLED"));

    if (!SlotWrapBox)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnsureSlotsInitialized: SlotWrapBox is NULL"));
        return;
    }

    if (!ItemBoxClass)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnsureSlotsInitialized: ItemBoxClass is NULL"));
        return;
    }



    SlotWrapBox->ClearChildren();
    WeaponSlots.Empty();



    for (int32 i = 0; i < NumEquipmentSlots; ++i)
    {
        UVMInventoryItemSlot* NewSlotWidget =
            CreateWidget<UVMInventoryItemSlot>(this, ItemBoxClass);

        UE_LOG(LogTemp, Warning,
            TEXT("EnsureSlotsInitialized: CreateWidget idx=%d -> %s"),
            i, *GetNameSafe(NewSlotWidget));

        if (!NewSlotWidget)
            continue;

        NewSlotWidget->SlotType = ESlotType::Equipment;

        SlotWrapBox->AddChildToWrapBox(NewSlotWidget);
        WeaponSlots.Add(NewSlotWidget);

        // 장비 없는 상태로 초기화
        NewSlotWidget->SetItemReference(nullptr);
    }

    UE_LOG(LogTemp, Warning,
        TEXT("EnsureSlotsInitialized DONE, WeaponSlots.Num=%d"),
        WeaponSlots.Num());
}

int32 UVMEquipmentPanel::TryEquipToEmptySlot(UVMEquipment* Item)
{
    UE_LOG(LogTemp, Warning,
        TEXT("EquipmentPanel::TryEquipToEmptySlot called. WeaponSlots.Num=%d, Item=%s"),
        WeaponSlots.Num(),
        *GetNameSafe(Item));

    if (!Item)
        return INDEX_NONE;

    // 슬롯이 아직 안 만들어졌다면 여기서라도 생성
    EnsureSlotsInitialized();

    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        UVMInventoryItemSlot* EquipSlot = WeaponSlots[i];
        if (!EquipSlot)
            continue;

        UE_LOG(LogTemp, Warning,
            TEXT("EquipmentPanel: Check Slot %d: Widget=%s ItemRef=%p"),
            i,
            *GetNameSafe(EquipSlot),
            EquipSlot->GetItemReference());

        if (EquipSlot->GetItemReference() == nullptr)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("EquipmentPanel: EMPTY SLOT FOUND -> %d"), i);

            // 인벤토리랑 동일하게 아이콘/텍스트 세팅
            EquipSlot->SetItemReference(Item);

            return i;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: No free equipment slot"));
    return INDEX_NONE;
}

void UVMEquipmentPanel::ClearAllSlots()
{
    EnsureSlotsInitialized();

    for (UVMInventoryItemSlot* EquipSlot : WeaponSlots)
    {
        if (EquipSlot)
        {
            EquipSlot->SetItemReference(nullptr);
        }
    }
}

void UVMEquipmentPanel::HandleEquipmentSlotDoubleClicked(UVMEquipment* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: DoubleClick but Item is NULL"));
        return;
    }

    // Owner 캐릭터 가져오기
    APlayerController* PC = GetOwningPlayer();
    if (!PC)
        return;

    AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(PC->GetPawn());
    if (!Hero)
        return;

    // 1) 장비 해제 (캐릭터 스탯 감소 처리)
    Hero->UnequipItem(Item);

    // 2) 인벤토리에 다시 추가
    if (UVMInventoryComponent* Inv = Hero->GetInventory())
    {
        Inv->AddItem(Item);
    }

    // 3) 장비 패널에서 슬롯 비우기
    int32 SlotIndex = WeaponSlots.IndexOfByKey(Item);
    if (WeaponSlots.IsValidIndex(SlotIndex))
    {
        WeaponSlots[SlotIndex]->SetItemReference(nullptr);
    }

    // 4) 인벤토리 패널 새로고침
    if (AVMCharacterHeroHUD* HUD = PC->GetHUD<AVMCharacterHeroHUD>())
    {
        if (HUD->InventoryPanel)
        {
            HUD->InventoryPanel->RefreshInventory();
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: Unequipped %s & returned to inventory"),
        *Item->GetEquipmentInfo().ItemName);
}
