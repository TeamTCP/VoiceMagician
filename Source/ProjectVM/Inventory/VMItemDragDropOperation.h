// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "VMItemDragDropOperation.generated.h"

class UVMEquipment;
class UVMInventoryComponent;
/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UVMEquipment* SourceItem;

	UPROPERTY(BlueprintReadWrite)
	UVMInventoryComponent* SourceInventory;

	// 드래그를 시작한 슬롯(인벤토리 슬롯 위젯)
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<class UVMInventoryItemSlot> SourceSlot;

	// 실제로 끌고 있는 아이템
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<UVMEquipment> DraggedItem;
};
