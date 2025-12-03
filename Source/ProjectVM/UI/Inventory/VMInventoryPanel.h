// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Inventory/VMInventoryComponent.h"

#include "VMInventoryPanel.generated.h"


class UVMInventoryItemSlot;
class UVMInventoryComponent;
class AVMCharacterHeroBase;
class UVMEquipmentItemSlot;
class UVMEquipmentPanel;


/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void RefreshInventory();


	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY()
	AVMCharacterHeroBase* Character;

	UPROPERTY()
	UVMInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVMInventoryItemSlot> InventorySlotClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PanelRefs")
	TObjectPtr<UVMEquipmentPanel> LinkedEquipmentPanel;

	UFUNCTION()
	void HandleItemDoubleClicked(class UVMEquipment* Item);

protected:



	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
