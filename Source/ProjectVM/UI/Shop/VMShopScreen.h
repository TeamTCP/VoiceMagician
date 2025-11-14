// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMShopScreen.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMShopScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UVMShopScreen(const FObjectInitializer& ObjectInitializer);
	//Initialize는 WBP 위젯과 연동 전에 호출될 수 있다. 안전하게 NativeConstruct()에서 초기화 진행
	virtual void NativeConstruct() override;

public:
	void SetShop(const TArray<FVMEquipmentInfo>& ShopItems);

	UFUNCTION()
	void OnGridItemButtonClicked(const FVMEquipmentInfo &ClickedItemInfo);

private:
	UFUNCTION()
	void OnBuyClicked();

	UFUNCTION()
	void OnSellClicked();

	UFUNCTION()
	void OnExitClicked();

	//버튼 스타일 변경 함수
	void UpdateButtonStyle();

public:
	// 유니폼 그리드 패널 참조
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> ShopGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> ShopListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BuyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SellButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	bool bIsBuy = true;

protected:
	TSubclassOf<class UVMShopItemWidget> ShopItemWidgetClass;
};
