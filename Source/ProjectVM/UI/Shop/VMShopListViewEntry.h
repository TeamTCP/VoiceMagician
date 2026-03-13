// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "VMShopListViewEntry.generated.h"

UCLASS()
class PROJECTVM_API UVMShopListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UVMShopListViewEntry(const FObjectInitializer& ObjectInitializer);
	//리스트뷰가 데이터를 연결할 때 마다 호출됨
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ChangeItemCount();

private:
	UFUNCTION()
	void OnListButtonClicked();
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterialInstance;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ItemButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCountText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemAllPriceText;

private:

	UPROPERTY()
	TObjectPtr<class UVMShopItemDataObject> ShopItemDataObject;

	//툴팁 위젯 관련
	UPROPERTY()
	TSubclassOf<class UVMShopItemTooltip> ItemTooltipClass;
	UPROPERTY()
	TObjectPtr<class UVMShopItemTooltip> ItemTooltip;
};
