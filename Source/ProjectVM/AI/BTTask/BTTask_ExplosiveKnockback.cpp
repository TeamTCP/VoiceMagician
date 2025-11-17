// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_ExplosiveKnockback.h"

#include "AOE/VMAOEExplosion.h"
#include "AIController.h"
#include "AI/Enemies/VMEnemyBoss.h"
#include "Projectile/VMStraightProjectile.h"
#include "Projectile/VMHomingProjectile.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Hero/VMCharacterHeroBase.h"
UBTTask_ExplosiveKnockback::UBTTask_ExplosiveKnockback()
{
}

EBTNodeResult::Type UBTTask_ExplosiveKnockback::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    // AI ControllerPtr
    AAIController* AIControllerPtr = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIControllerPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTTask_ExplosiveKnockback::ExecuteTask] AIController is nullptr"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Log, TEXT("OwnerCOmp.AIOwner:%s"), *AIControllerPtr->GetName());


    AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
    if (BossPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTTask_ExplosiveKnockback::ExecuteTask] BossPtr is nullptr"));
        return EBTNodeResult::Failed;
    }
    

    // 보스 위치에서 폭탄 설치할 예정.
    FVector SpawnLocation = BossPtr->GetActorLocation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FRotator SpawnRot(BossPtr->GetActorLocation().Rotation());

    AVMAOEExplosion* Explosion = GetWorld()->SpawnActor<AVMAOEExplosion>(AVMAOEExplosion::StaticClass(), SpawnLocation, SpawnRot, SpawnParams);
    if (Explosion == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    //AActor* Hero = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("EnemyTarget")));

    AVMCharacterHeroBase* HeroChar = Cast<AVMCharacterHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("EnemyTarget")));
    if (HeroChar == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    // Hero가 AOE 이벤트를 구독하도록 등록
    Explosion->OnAOEExplosionOverlapActor.RemoveDynamic(HeroChar, &AVMCharacterHeroBase::OnHitExplosionByAOE);
    Explosion->OnAOEExplosionOverlapActor.AddDynamic(HeroChar, &AVMCharacterHeroBase::OnHitExplosionByAOE);

    Explosion->SetRadius(1000);
    Explosion->TriggerSpawnAOESphere();

	return EBTNodeResult::Succeeded;
}
