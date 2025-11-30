// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SpawnBigFireOnCenter.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AOE/VMAOEFire.h"
#include "AOE/VMAOECenterFire.h"	// 이걸로 대체할 예정.

#include "Components/PrimitiveComponent.h"
#include "Core/VMLevelManager.h"


UBTTask_SpawnBigFireOnCenter::UBTTask_SpawnBigFireOnCenter()
{
}

EBTNodeResult::Type UBTTask_SpawnBigFireOnCenter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVMEnemyBoss* BossPawnPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
	if (BossPawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UVMLevelManager* LevelManager = GetWorld()->GetGameInstance()->GetSubsystem<UVMLevelManager>();
	if (LevelManager != nullptr)
	{
		ULevelStreaming* BossLevel = LevelManager->GetLevel(FName("BossMap"));
		if (BossLevel != nullptr && BossLevel->GetLoadedLevel() != nullptr)
		{
			Params.OverrideLevel = BossLevel->GetLoadedLevel();
			UE_LOG(LogTemp, Log, TEXT("Spawn location changed to BossMap"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("BossLevel is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("LevelManager is nullptr"));
	}

	
	FTransform Transform = BossPawnPtr->GetActorTransform();
	AVMAOECenterFire* FireSpawnActor = GetWorld()->SpawnActor<AVMAOECenterFire>(AVMAOECenterFire::StaticClass(), Transform, Params);
	if (FireSpawnActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("센터 불 소환 실패"));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
