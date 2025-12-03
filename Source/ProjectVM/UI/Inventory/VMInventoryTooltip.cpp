// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMInventoryTooltip.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "Hero/HeroStat.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Hero/VMHeroStatComponent.h"

#include "Components/TextBlock.h"

void UVMInventoryTooltip::NativeConstruct()
{
    Super::NativeConstruct();

    if (!InventorySlotBeingHovered)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("InventoryTooltip::NativeConstruct - InventorySlotBeingHovered is NULL"));
        return;
    }

    // 슬롯에서 현재 아이템 참조 가져오기
    UVMEquipment* EquipItem =
        Cast<UVMEquipment>(InventorySlotBeingHovered->GetItemReference());

    if (!EquipItem)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("InventoryTooltip::NativeConstruct - ItemReference is NULL or not UVMEquipment"));
        return;
    }

    const FVMEquipmentInfo& Info = EquipItem->GetEquipmentInfo();

    // ─────────── 기본 정보 ───────────
    ItemName->SetText(FText::FromString(Info.ItemName));
    ItemDesc->SetText(FText::FromString(Info.ItemDesc));

    // ─────────── 주요 스탯 표시 ───────────
    // DamageValue/ArmorRating 텍스트박스를 공격력/방어력으로 사용
    if (DamageValue)
    {
        if (Info.AttackPower != 0)
        {
            DamageValue->SetVisibility(ESlateVisibility::Visible);
            DamageValue->SetText(FText::FromString(FString::Printf(TEXT("공격력 %+d"), Info.AttackPower)));
        }
        else
        {
            DamageValue->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (ArmorRating)
    {
        if (Info.DefensivePower != 0)
        {
            ArmorRating->SetVisibility(ESlateVisibility::Visible);
            ArmorRating->SetText(FText::FromString(FString::Printf(TEXT("방어력 %+d"), Info.DefensivePower)));
        }
        else
        {
            ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    // 나머지 HP / MP / 이동속도 / 흡혈 등을 한 줄로 모아서 UsageText 에 보여주는 예시
    if (UsageText)
    {
        FString StatLines;

        auto AppendLine = [&StatLines](const TCHAR* Label, int32 Value)
            {
                if (Value != 0)
                {
                    if (!StatLines.IsEmpty())
                    {
                        StatLines += TEXT("\n");
                    }
                    StatLines += FString::Printf(TEXT("%s %+d"), Label, Value);
                }
            };

        AppendLine(TEXT("HP"), Info.HealthPoint);
        AppendLine(TEXT("MP"), Info.ManaPoint);
        AppendLine(TEXT("MP Regen"), Info.ManaRegeneration);
        AppendLine(TEXT("Speed"), Info.Speed);
        AppendLine(TEXT("Life Steal"), Info.LifeSteal);

        if (!Info.Ability.IsEmpty())
        {
            if (!StatLines.IsEmpty())
            {
                StatLines += TEXT("\n");
            }
            StatLines += FString::Printf(TEXT("Ability: %s"), *Info.Ability);
        }

        if (StatLines.IsEmpty())
        {
            // 추가 스탯이 하나도 없으면 숨기거나 기본 설명만 보여줄 수도 있음
            UsageText->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            UsageText->SetVisibility(ESlateVisibility::Visible);
            UsageText->SetText(FText::FromString(StatLines));
        }
    }

}
