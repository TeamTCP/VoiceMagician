// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMInventoryTooltip.generated.h"

class UVMInventoryItemSlot;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	//UInventoryItemSlot* InventorySlotBeingHovered;
	TObjectPtr<UVMInventoryItemSlot> InventorySlotBeingHovered;

	UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> ItemName;
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ArmorRating;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UsageText;


protected:
	virtual void NativeConstruct() override;
	
};
