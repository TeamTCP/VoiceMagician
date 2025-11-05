// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFactorySubsystem.h"
#include "Item/Equipment/VMEquipment.h"

UVMEquipment* UItemFactorySubsystem::CreateRandomBaseEquipment()
{
	UVMEquipment* NewEquipment = NewObject<UVMEquipment>();

	int32 RandomIndex = FMath::RandHelper(BaseEquipmentInfo.Num());
	NewEquipment->SetEquipmentInfo(BaseEquipmentInfo[RandomIndex]);

	return NewEquipment;
}

UVMEquipment* UItemFactorySubsystem::CreateEquipmentByName(FName Name)
{
	UVMEquipment* NewEquipment = NewObject<UVMEquipment>();

	FVMEquipmentInfo* FoundInfo = EquipmentInfoByName.Find(Name);
	if (FoundInfo == nullptr) return nullptr;
	
	NewEquipment->SetEquipmentInfo(*FoundInfo);
	
	return NewEquipment;
}

UVMEquipment* UItemFactorySubsystem::CraftEquipment(UVMEquipment* BaseEquipment1, UVMEquipment* BaseEquipment2)
{
	if (BaseEquipment1 == nullptr) return nullptr;
	if (BaseEquipment2 == nullptr) return nullptr;

	UVMEquipment* NewEquipment = NewObject<UVMEquipment>();

	FVMEquipmentInfo EquipmentInfo1 = BaseEquipment1->GetEquipmentInfo();
	FVMEquipmentInfo EquipmentInfo2 = BaseEquipment2->GetEquipmentInfo();

	if (EquipmentInfo1.ItemLevel != 1) return nullptr;
	if (EquipmentInfo2.ItemLevel != 1) return nullptr;
	
	int32 CombID = EquipmentInfo1.ItemID | EquipmentInfo2.ItemID;
	FVMEquipmentInfo* CombInfoPtr = EquipmentInfoByID.Find(CombID);
	if (CombInfoPtr == nullptr) return nullptr;

	NewEquipment->SetEquipmentInfo(*CombInfoPtr);

	return NewEquipment;
}

void UItemFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UDataTable* EquipmentDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Project/Item/VMEquipmentInfo.VMEquipmentInfo"));
	if (EquipmentDataTable == nullptr) return;

	for (const FName& RowName : EquipmentDataTable->GetRowNames())
	{
		FVMEquipmentInfo* EquipmentInfoPtr = EquipmentDataTable->FindRow<FVMEquipmentInfo>(RowName, TEXT(""));
		if (EquipmentInfoPtr == nullptr) continue;

		AllEquipmentInfo.Add(*EquipmentInfoPtr);
		if (EquipmentInfoPtr->ItemLevel == 1) BaseEquipmentInfo.Add(*EquipmentInfoPtr);

		EquipmentInfoByName.Add(FName(EquipmentInfoPtr->ItemName), *EquipmentInfoPtr);
		EquipmentInfoByID.Add(EquipmentInfoPtr->ItemID, *EquipmentInfoPtr);
	}
}
