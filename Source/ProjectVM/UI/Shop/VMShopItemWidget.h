// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMShopItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMShopItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUp(const FVMEquipmentInfo& Info);

	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnItemButtonClicked();
public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterialInstance;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameText;

	UPROPERTY()
	TObjectPtr<class UVMShopScreen> ShopScreen;

private:
	const FVMEquipmentInfo* EquipmentInfo; //아이템 구조체를 참조해서 가지고 있는다.
};
