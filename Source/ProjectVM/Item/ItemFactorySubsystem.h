// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemFactorySubsystem.generated.h"

/**
 * 
 */

class UVMEquipment;

UCLASS()
class PROJECTVM_API UItemFactorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UVMEquipment* CreateRandomBaseEquipment();
	UVMEquipment* CreateEquipmentByName(FName Name);
	UVMEquipment* CraftEquipment(UVMEquipment* BaseEquipment1, UVMEquipment* BaseEquipment2);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TArray<FVMEquipmentInfo> AllEquipmentInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TArray<FVMEquipmentInfo> BaseEquipmentInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TMap<FName, FVMEquipmentInfo> EquipmentInfoByName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TMap<int32, FVMEquipmentInfo> EquipmentInfoByID;
};
