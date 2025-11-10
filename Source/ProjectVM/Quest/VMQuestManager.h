// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/VMQuestData.h"
#include "VMQuestManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestPublished, const FVMQuestData&);
UCLASS()
class PROJECTVM_API UVMQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//NPC 한테 퀘스트 전달
	void AssignQuestToNPC(FName QuestId);

	// 퀘스트 수락
	void AcceptQuest(FVMQuestData QuestData);
public:
	FOnQuestPublished OnQuestPublished;
};
