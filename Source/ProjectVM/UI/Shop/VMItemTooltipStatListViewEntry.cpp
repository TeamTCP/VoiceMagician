
#include "VMItemTooltipStatListViewEntry.h"
#include "VMItemTooltipStatDataObject.h"
#include "Components/TextBlock.h"

void UVMItemTooltipStatListViewEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UVMItemTooltipStatDataObject* StatDataObject = Cast<UVMItemTooltipStatDataObject>(ListItemObject);
	if (StatDataObject != nullptr && ItemStat != nullptr)
	{
		ItemStat->SetText(StatDataObject->ItemStatText);
		ItemStat->SetColorAndOpacity(StatDataObject->TextColor);
	}
}
