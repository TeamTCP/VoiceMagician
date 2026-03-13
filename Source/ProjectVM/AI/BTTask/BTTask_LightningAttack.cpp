// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_LightningAttack.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_LightningAttack.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AOE/VMAOELightning.h"
#include "AOE/VMAOEMeteor.h"
#include "AOE/VMAOEFrozen.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Core/VMLevelManager.h"

#include "Kismet/GameplayStatics.h"

UBTTask_LightningAttack::UBTTask_LightningAttack()
{

}

void UBTTask_LightningAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (UWorld* World = OwnerComp.GetWorld())
	{
		World->GetTimerManager().ClearTimer(ThunderTimer);
	}
}

EBTNodeResult::Type UBTTask_LightningAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIControllerPtr = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIControllerPtr == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTTask_FireStraightProjectile::ExecuteTask] AIController is nullptr"));
		return EBTNodeResult::Failed;
	}


	AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
	if (BossPtr == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTTask_FireStraightProjectile::ExecuteTask] BossPtr is nullptr"));
		return EBTNodeResult::Failed;
	}


	return SpawnThunderToTarget(OwnerComp, BossPtr);
}

EBTNodeResult::Type UBTTask_LightningAttack::SpawnThunderToTarget(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
	if (BossPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = BossPtr->GetWorld();
	if (World == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 초기화
	SpawnFinishedCount = 0;
	SpawnTotalCount = FMath::RandRange(5, 10);

	// OwnerComp를 포인터로 캡처
	UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;

	// 나머지는 타이머로 순차 발사
	World->GetTimerManager().SetTimer(ThunderTimer,
		[this, OwnerCompPtr, BossPtr, World]()
		{
			// 유효성 검사
			if (!IsValid(BossPtr) || !IsValid(OwnerCompPtr) || !World)
			{
				World->GetTimerManager().ClearTimer(ThunderTimer);
				return;
			}

			// 매 발사 시점에 랜덤 X,Y 생성
			UBlackboardComponent* BBComp = OwnerCompPtr->GetBlackboardComponent();
			if (BBComp == nullptr)
			{
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
				return;
			}

			AVMCharacterHeroBase* HeroPawnTarget = Cast<AVMCharacterHeroBase>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
			if (HeroPawnTarget == nullptr)
			{
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
				return;
			}


			FTransform Transform = HeroPawnTarget->GetActorTransform();

			// BossMap Level 찾기
			UVMLevelManager* LevelManager = World->GetGameInstance()->GetSubsystem<UVMLevelManager>();

			ULevel* TargetLevel = nullptr;

			if (LevelManager == nullptr)
			{
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
				return;
			}
			ULevelStreaming* BossLevel = LevelManager->GetLevel(FName("BossMap"));

			if (BossLevel && BossLevel->GetLoadedLevel())
			{
				TargetLevel = BossLevel->GetLoadedLevel();
			}

			// SpawnParams 직접 구성
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnParams.Owner = BossPtr;
			SpawnParams.Instigator = BossPtr;
			SpawnParams.bDeferConstruction = true;

			if (TargetLevel == nullptr)
			{
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
				return;
			}
			SpawnParams.OverrideLevel = TargetLevel;

			// SpawnActor (Deferred 직접 구현)
			AVMAOELightning* Thunder = World->SpawnActor<AVMAOELightning>(AVMAOELightning::StaticClass(), Transform, SpawnParams);
			if (Thunder == nullptr)
			{
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
				return;
			}

			Thunder->OnAOEThunderOverlapActor.RemoveDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitThunderByAOE);
			Thunder->OnAOEThunderOverlapActor.AddDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitThunderByAOE);

			Thunder->SetDelay(3.0f);
			UGameplayStatics::FinishSpawningActor(Thunder, Transform);

			SpawnFinishedCount++;

			// 마지막 발사 후 종료
			if (SpawnFinishedCount >= SpawnTotalCount)
			{
				World->GetTimerManager().ClearTimer(ThunderTimer);
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
			}
		},
		1.0f,
		true
	);

	return EBTNodeResult::InProgress;
}
