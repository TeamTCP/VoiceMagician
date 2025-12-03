// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMEquipmentItemSlot.generated.h"

class UVMEquipmnet;
class UImage;
class UBorder;
class UMaterialInstanceDynamic;


/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMEquipmentItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    UVMEquipment* GetItem() const { return ItemReference; }

    UFUNCTION(BlueprintCallable)
    void SetItem(UVMEquipment* InItem);

    UFUNCTION(BlueprintCallable)
    void ClearItem();

public:

    UPROPERTY(VisibleAnywhere, Category = "Equipment Slot")
    TObjectPtr<UVMEquipment> ItemReference;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemIcon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> ItemBorder;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> ItemMaterialInstance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemIcon")
    TObjectPtr<UMaterialInterface> AtlasMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemIcon")
    TObjectPtr<UMaterialInterface> ItemIconBaseMaterial;

    //void SetUp(const FVMEquipmentInfo& Info);

    virtual void NativeConstruct() override;
    void RefreshFromItem();

};
