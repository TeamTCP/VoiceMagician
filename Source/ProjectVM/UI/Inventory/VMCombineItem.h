// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMCombineItem.generated.h"


class UButton;
class UVMCombineSlot;
class UVMInventoryComponent;
/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMCombineItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 조합 슬롯 A
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combine")
    TObjectPtr<class UVMCombineSlot> CombineSlotA;

    // 조합 슬롯 B
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combine")
    TObjectPtr<class UVMCombineSlot> CombineSlotB;

    // 조합 버튼
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combine")
    TObjectPtr<class UButton> CombineButton;

    UVMInventoryComponent* GetInventoryComponent() const; // 캐릭터에서 찾아오는 헬퍼

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleCombineClicked();

    
};
