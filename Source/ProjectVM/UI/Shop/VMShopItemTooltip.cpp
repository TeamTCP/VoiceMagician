#include "VMShopItemTooltip.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "VMItemTooltipStatDataObject.h"

void UVMShopItemTooltip::Setup(const FVMEquipmentInfo* Info)
{
	if (Info == nullptr)
	{
		return;
	}
	if (ItemName != nullptr)
	{
		ItemName->SetText(FText::FromString(Info->ItemName));
	}
	if (ItemStatListView != nullptr)
	{
        // 스탯 이름과 값, 포맷 정보를 배열로 저장
        struct FStatInfo
        {
            FString Name;
            int32 Value;
			FLinearColor TextColor;
        };

        TArray<FStatInfo> Stats;
        Stats.Add({ "Attack", Info->AttackPower, FLinearColor::Red});						//공격력, 빨강
		Stats.Add({ "Defense", Info->DefensivePower, FLinearColor::Blue });					//방어력, 파랑
		Stats.Add({ "HP", Info->HealthPoint, FLinearColor::Green });						//HP, 초록
		Stats.Add({ "MP", Info->ManaPoint, FLinearColor(0.f, 1.f, 1.f) });					//MP, 하늘색
		Stats.Add({ "MP Regen", Info->ManaRegeneration, FLinearColor(1.f, 0.5f, 0.f) });	//쿨타임, 주황
		Stats.Add({ "Speed", Info->Speed, FLinearColor::Yellow });							//속도, 노랑
		Stats.Add({ "LifeSteal", Info->LifeSteal, FLinearColor(0.5f, 0.f, 0.5f) });			//흡혈, 보라

        // 반복문으로 0이 아닌 스탯만 리스트뷰에 추가
        for (const FStatInfo& Stat : Stats)
        {
            if (Stat.Value > 0)
            {
                UVMItemTooltipStatDataObject* NewStatDataObject = NewObject<UVMItemTooltipStatDataObject>(this);

                FString StatString = FString::Printf(TEXT("%s +%d"), *Stat.Name, Stat.Value);
                NewStatDataObject->ItemStatText = FText::FromString(StatString);
				NewStatDataObject->TextColor = Stat.TextColor;

                ItemStatListView->AddItem(NewStatDataObject);
            }
        }

	}
	if (ItemDesc != nullptr)
	{
		ItemDesc->SetText(FText::FromString(Info->ItemDesc));
	}
}
