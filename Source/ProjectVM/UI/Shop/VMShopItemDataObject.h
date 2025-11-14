
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMShopItemDataObject.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnItemCountUpdated);
UCLASS()
class PROJECTVM_API UVMShopItemDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	void ChangeItemCount(bool bIsAdd)
	{
		//아이템 추가
		if (bIsAdd)
		{
			//한 번에 살 수 있는 아이템 수 99개로 제한
			CurrentCount = CurrentCount >= 99 ? 99 : CurrentCount + 1;
		}
		//아이템 삭제
		else
		{
			//Todo: 아이템 삭제 구현 (이상윤)
		}
		OnItemCountUpdated.Broadcast(); //위젯에 변경 사항 알림.
	}

public:
	FOnItemCountUpdated OnItemCountUpdated;

	const FVMEquipmentInfo* EquipmentInfo;

	UPROPERTY()
	int32 CurrentCount = 1;
};
