// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMShopItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMShopItemTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(const FVMEquipmentInfo* Info);
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> ItemName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UListView> ItemStatListView;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemDesc;
};
