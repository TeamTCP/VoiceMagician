// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMCombineItem.h"
#include "UI/Inventory/VMCombineSlot.h"

#include "Inventory/VMInventoryComponent.h"
#include "Hero/VMCharacterHeroBase.h"

#include "GameFramework/PlayerController.h"
#include "Components/Button.h"

void UVMCombineItem::NativeConstruct()
{
    Super::NativeConstruct();

    if (CombineButton)
    {
        CombineButton->OnClicked.AddDynamic(
            this, &UVMCombineItem::HandleCombineClicked);
    }
}


UVMInventoryComponent* UVMCombineItem::GetInventoryComponent() const
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AVMCharacterHeroBase* Character = Cast<AVMCharacterHeroBase>(PC->GetPawn()))
        {
            return Character->GetInventory(); // 네 캐릭터 함수에 맞게 수정
        }
    }
    return nullptr;
}

void UVMCombineItem::HandleCombineClicked()
{

    if (!CombineSlotA || !CombineSlotB)
        return;

    UVMInventoryComponent* InvComp = GetInventoryComponent();
    if (!InvComp)
        return;

    InvComp->CombineItems(CombineSlotA->StoredItem, CombineSlotB->StoredItem);
}