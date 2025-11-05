// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "UObject/NoExportTypes.h"
#include "VMEquipment.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTVM_API UVMEquipment : public UObject
{
	GENERATED_BODY()

public:
	void EquipEffect(class AVMCharacterHeroBase* Owner);
	FORCEINLINE FVMEquipmentInfo GetEquipmentInfo() { return EquipmentInfo; }
	
protected:
	friend class UItemFactorySubsystem;
	FORCEINLINE void SetEquipmentInfo(FVMEquipmentInfo InEquipmentInfo) { EquipmentInfo = InEquipmentInfo; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FVMEquipmentInfo EquipmentInfo;
};
