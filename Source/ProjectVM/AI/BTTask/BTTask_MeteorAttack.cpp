// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_MeteorAttack.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_LightningAttack.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AOE/VMAOEMeteor.h"
#include "AOE/VMAOEFire.h"  // TODO 삭제?

#include "Hero/VMCharacterHeroBase.h"

#include "Core/VMLevelManager.h"

#include "Kismet/GameplayStatics.h"

UBTTask_MeteorAttack::UBTTask_MeteorAttack()
{

}

EBTNodeResult::Type UBTTask_MeteorAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


    return SpawnMeteorToTarget(OwnerComp, BossPtr);
}

EBTNodeResult::Type UBTTask_MeteorAttack::SpawnMeteorToTarget(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
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

    // 초기화
    SpawnFinishedCount = 0;
    SpawnTotalCount = FMath::RandRange(5, 10);

    // 나머지는 타이머로 순차 발사
    World->GetTimerManager().SetTimer(MeteorTimer,
        [this, &OwnerComp, BossPtr, World]()
        {
            if (BossPtr == nullptr || World == nullptr)
            {
                World->GetTimerManager().ClearTimer(MeteorTimer);
                return;
            }

            UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
            if (BBComp == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }

            AVMCharacterHeroBase* HeroPawnTarget = Cast<AVMCharacterHeroBase>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
            if (HeroPawnTarget == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }


            FTransform Transform = HeroPawnTarget->GetActorTransform();

            // BossMap Level 찾기
            UVMLevelManager* LevelManager = World->GetGameInstance()->GetSubsystem<UVMLevelManager>();

            ULevel* TargetLevel = nullptr;

            if (LevelManager == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
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
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }
            SpawnParams.OverrideLevel = TargetLevel;

            // SpawnActor (Deferred 직접 구현)
            AVMAOEMeteor* SpawnMeteorActorPtr = World->SpawnActor<AVMAOEMeteor>(AVMAOEMeteor::StaticClass(), Transform, SpawnParams);
            if (SpawnMeteorActorPtr == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }
            
            // 이벤트 제거 후 다시 삽입.
            SpawnMeteorActorPtr->OnAOEMeteorOverlapActor.RemoveDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitMeteorByAOE);
            SpawnMeteorActorPtr->OnAOEMeteorOverlapActor.AddDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitMeteorByAOE);


            //SpawnMeteorActorPtr->SetDelay(3.0f);
            UGameplayStatics::FinishSpawningActor(SpawnMeteorActorPtr, Transform);

            SpawnFinishedCount++;

            // 마지막 발사 후 종료
            if (SpawnFinishedCount >= SpawnTotalCount)
            {
                World->GetTimerManager().ClearTimer(MeteorTimer);
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }
        },
        1.0f,
        true
    );

    return EBTNodeResult::InProgress;
}
