// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/VMQuestData.h"
#include "VMQuestDataObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMQuestDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FVMQuestData QuestData;
};
