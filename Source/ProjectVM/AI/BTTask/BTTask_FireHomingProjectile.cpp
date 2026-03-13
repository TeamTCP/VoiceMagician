// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FireHomingProjectile.h"
#include "BTTask_FireHomingProjectile.h"

#include "AIController.h"
#include "AI/Enemies/VMEnemyBoss.h"
#include "Projectile/VMStraightProjectile.h"
#include "Projectile/VMHomingProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/VMLevelManager.h"

UBTTask_FireHomingProjectile::UBTTask_FireHomingProjectile()
{
}

void UBTTask_FireHomingProjectile::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}


EBTNodeResult::Type UBTTask_FireHomingProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // TEST
	AActor* AcotrPtr = OwnerComp.GetOwner();
	if (AcotrPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

    UE_LOG(LogTemp, Log, TEXT("OwnerCOmp.Owner:%s"), *AcotrPtr->GetName());

    AAIController* AIControllerPtr = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIControllerPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTTask_FireHomingProjectile::ExecuteTask] AIController is nullptr"));
        return EBTNodeResult::Failed;
    }


    AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
    if (BossPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTTask_FireHomingProjectile::ExecuteTask] BossPtr is nullptr"));
        return EBTNodeResult::Failed;
    }
    
    int32 Index = FMath::RandRange(0, 1);
    switch (Index)
    {
    case 0:
        return SpawnAroundAll(OwnerComp, BossPtr);
    case 1:
        return SpawnForwardRand(OwnerComp, BossPtr);
    default:
        break;
    }

    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_FireHomingProjectile::SpawnAroundAll(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
    const int32 ProjectileCount = 12;
    const float AngleStep = 360.0f / ProjectileCount;

    //보스 위치에서 스폰하도록
    FVector SpawnLocation = BossPtr->GetActorLocation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < ProjectileCount; i++)
    {
        // 각 발사체의 Yaw 각도 계산
        float YawAngle = i * AngleStep;

        // 발사체 회전 (Pitch=0, Roll=0, Yaw만 변화)
        FRotator SpawnRot = FRotator(0.f, YawAngle, 0.f);

        // 회전으로부터 전방 방향 벡터 구함
        FVector Dir = SpawnRot.Vector();

        // 발사체가 향하는 방향으로 1000cm 앞에서 스폰
        FVector FinalSpawnLocation = SpawnLocation + Dir * 1000.0f;

        AVMHomingProjectile* Projectile = GetWorld()->SpawnActor<AVMHomingProjectile>(AVMHomingProjectile::StaticClass(), FinalSpawnLocation, SpawnRot, SpawnParams);
        if (Projectile == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Projectile Spawn Failed at %d"), i);
        }

        // 타겟 설정.
        UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
        AActor* PawnActorPtr = Cast<AActor>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
        Projectile->SetTarget(PawnActorPtr);
    }
    
    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_FireHomingProjectile::SpawnForwardRand(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
    const int32 ProjectileCount = 3;
    const float AngleStep = FMath::FRandRange<float>(30, 60);

    //보스 위치에서 스폰하도록
    FVector SpawnLocation = BossPtr->GetActorLocation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    
    UVMLevelManager* LevelManager = GetWorld()->GetGameInstance()->GetSubsystem<UVMLevelManager>();
    if (LevelManager != nullptr)
    {
        ULevelStreaming* BossLevel = LevelManager->GetLevel(FName("BossMap"));
        if (BossLevel != nullptr && BossLevel->GetLoadedLevel() != nullptr)
        {
            SpawnParams.OverrideLevel = BossLevel->GetLoadedLevel();
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
    
    
    
    // 타겟 설정.
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    AActor* PawnActorPtr = Cast<AActor>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));


    const float CenterYaw = BossPtr->GetActorRotation().Yaw;
    const float OffsetAngle = AngleStep; // 좌우 발사 각도 (원하는 만큼 조정)
    const float ForwardDistance = 300.f;
    {
        FRotator CenterRot = FRotator(0.f, CenterYaw, 0.f);
        FVector Dir = CenterRot.Vector();
        FVector FinalSpawnLocation = SpawnLocation + Dir * ForwardDistance;

        AVMHomingProjectile* Projectile = GetWorld()->SpawnActor<AVMHomingProjectile>(AVMHomingProjectile::StaticClass(), FinalSpawnLocation, CenterRot, SpawnParams);

        if (Projectile)
        {
            Projectile->SetTarget(PawnActorPtr);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Center Projectile spawn failed"));
        }
    }

    // 2. 좌우 2발
    for (int32 Side = -3; Side <= 3; Side += 2) // -1(left), +1(right)
    {
        float Yaw = CenterYaw + (OffsetAngle * Side);
        FRotator SpawnRot = FRotator(0.f, Yaw, 0.f);
        FVector Dir = SpawnRot.Vector();
        FVector FinalSpawnLocation = SpawnLocation + Dir * ForwardDistance;

        AVMHomingProjectile* Projectile = GetWorld()->SpawnActor<AVMHomingProjectile>(AVMHomingProjectile::StaticClass(),FinalSpawnLocation, SpawnRot, SpawnParams);

        if (Projectile)
        {
            Projectile->SetTarget(PawnActorPtr);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Side Projectile spawn failed (Side=%d)"), Side);
        }
    }
    return EBTNodeResult::Succeeded;
}
