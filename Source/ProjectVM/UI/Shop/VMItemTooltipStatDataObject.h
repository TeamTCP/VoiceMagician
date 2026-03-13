#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VMItemTooltipStatDataObject.generated.h"

UCLASS()
class PROJECTVM_API UVMItemTooltipStatDataObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FText ItemStatText;
	UPROPERTY()
	FLinearColor TextColor;
};
