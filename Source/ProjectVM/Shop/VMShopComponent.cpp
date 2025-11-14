
#include "Shop/VMShopComponent.h"
#include "Game/VMRPGPlayerController.h"
#include "UI/Shop/VMShopScreen.h"
#include "Core/VMLoadManager.h"

UVMShopComponent::UVMShopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVMShopComponent::SetShop()
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}

	UVMShopScreen* ShopScreen = Cast<UVMShopScreen>(PC->GetScreen(EScreenUIType::ShopScreen));
	if (ShopScreen == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ShopScreen is not found"));
		return;
	}
	ShopScreen->SetShop(ShopItems);
}

void UVMShopComponent::BeginPlay()
{
	Super::BeginPlay();

	//아이템 추가
	for (int i = 1; i < 28; ++i)
	{
		AddShopItem(i);
	}
}

void UVMShopComponent::AddShopItem(int32 ItemNum)
{
	FName RowName = FName(*FString::FromInt(ItemNum));
	FVMEquipmentInfo* LoadedData = GetOwner()->GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetEquipmentInfoRow(RowName);
	if (LoadedData)
	{
		ShopItems.Add(*LoadedData); // 포인터 → 값 복사
	}
}




