#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "VMItemTooltipStatListViewEntry.generated.h"


UCLASS()
class PROJECTVM_API UVMItemTooltipStatListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	//리스트뷰가 데이터를 연결할 때 마다 호출됨
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemStat;
};
