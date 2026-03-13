// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FrozenAttack.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_LightningAttack.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AOE/VMAOEFrozen.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Core/VMLevelManager.h"

#include "Kismet/GameplayStatics.h"

UBTTask_FrozenAttack::UBTTask_FrozenAttack()
{

}

void UBTTask_FrozenAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (UWorld* World = OwnerComp.GetWorld())
	{
		World->GetTimerManager().ClearTimer(FrozenTimer);
	}
}

EBTNodeResult::Type UBTTask_FrozenAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UE_LOG(LogTemp, Log, TEXT("왔으면 좋겠네1."));
	return SpawnFrozenToTarget(OwnerComp, BossPtr);
}

EBTNodeResult::Type UBTTask_FrozenAttack::SpawnFrozenToTarget(UBehaviorTreeComponent& OwnerComp, class AVMEnemyBoss* BossPtr)
{
	// 보스가 없는 경우(이미 위에서 걸리지긴 함)
	if (BossPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = BossPtr->GetWorld();
	// 월드가 없는 경우
	if (World == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Log, TEXT("왔으면 좋겠네."));
	// 초기화
	SpawnFinishedCount = 0;

	SpawnTotalCount = FMath::RandRange(5, 10);

	// OwnerComp를 포인터로 캡처
	UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;

	// 나머지는 타이머로 순차 발사
	World->GetTimerManager().SetTimer(FrozenTimer,
		[this, OwnerCompPtr, BossPtr, World]()
		{
			// 유효성 검사
			if (!IsValid(BossPtr) || !IsValid(OwnerCompPtr) || !World)
			{
				World->GetTimerManager().ClearTimer(FrozenTimer);
				return;
			}

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
			UE_LOG(LogTemp, Log, TEXT("Target Transform: (%f, %f, %f)"), Transform.GetLocation().X, Transform.GetLocation().Y, Transform.GetLocation().Z);

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
			AVMAOEFrozen* SpawnFrozenActorPtr = World->SpawnActor<AVMAOEFrozen>(AVMAOEFrozen::StaticClass(), Transform, SpawnParams);

			if (SpawnFrozenActorPtr == nullptr)
			{
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
				return;
			}
			//RemoveDynamic
			SpawnFrozenActorPtr->OnAOEFrozenOverlapActor.RemoveDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitFrozenByAOE);
			SpawnFrozenActorPtr->OnAOEFrozenOverlapActor.AddDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitFrozenByAOE);

			UGameplayStatics::FinishSpawningActor(SpawnFrozenActorPtr, Transform);

			SpawnFinishedCount++;

			// 마지막 발사 후 종료
			if (SpawnFinishedCount >= SpawnTotalCount)
			{
				World->GetTimerManager().ClearTimer(FrozenTimer);
				FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
			}
		},
		1.0f,
		true
	);

	return EBTNodeResult::InProgress;
}