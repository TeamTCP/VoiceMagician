#include "Quest/VMQuestManager.h"
#include "Core/VMLoadManager.h"
#include "NPC/VMNPC.h"

void UVMQuestManager::AssignQuestToNPC(FName QuestId)
{
	FVMQuestData* NowQuestData = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetQuestDataRow(QuestId);
	if (NowQuestData != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Quest : %s"), *NowQuestData->QuestTitle.ToString());
		// 퀘스트 데이터 전달 (델리게이트 브로드캐스트)
		OnQuestPublished.Broadcast(*NowQuestData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest Not Found: %s"), *QuestId.ToString());
	}
}

void UVMQuestManager::AcceptQuest(FVMQuestData QuestData)
{
	UE_LOG(LogTemp, Log, TEXT("퀘스트 수락"));
}
