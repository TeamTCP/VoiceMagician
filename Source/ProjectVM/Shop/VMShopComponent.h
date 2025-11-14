#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMShopComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVMShopComponent();

	void SetShop();
	
protected:
	virtual void BeginPlay() override;
	
private:
	void AddShopItem(int32 ItemNum);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	FName ShopId = TEXT("temp");
	
private:
	UPROPERTY()
	TArray<struct FVMEquipmentInfo> ShopItems;
};
