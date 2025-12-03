// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hero/VMCharacterHeroBase.h"
#include "VMMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	//AVMPracCharacter* PlayerCharacter;
	TObjectPtr<AVMCharacterHeroBase> Character;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVMInventoryPanel> WBP_VMInventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVMEquipmentPanel> WBP_VMEquipmentPanel;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
