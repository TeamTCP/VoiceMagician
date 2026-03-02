// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMCombineSlot.generated.h"


class UVMInventoryItemSlot;
class UVMEquipment;
class UBorder;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMCombineSlot : public UUserWidget
{
	GENERATED_BODY()

public:
    void RefreshFromStoredItem();
	
public:
    // 실제로 조합에 사용할 아이템
    UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combine")
    TObjectPtr<UVMEquipment> StoredItem;

protected:
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UBorder> DropBorder;    // 시각용

    //UPROPERTY(meta = (BindWidgetOptional))
    //TObjectPtr<UImage> ItemIcon;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UVMInventoryItemSlot> InnerItemSlot;


    virtual bool NativeOnDrop(
        const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;
};
