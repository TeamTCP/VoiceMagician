// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FireStraightProjectile.h"

#include "AIController.h"

#include "Projectile/VMStraightProjectile.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Core/VMLevelManager.h"

UBTTask_FireStraightProjectile::UBTTask_FireStraightProjectile()
{
}

EBTNodeResult::Type UBTTask_FireStraightProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    
    int32 Index = FMath::RandRange(0, 4);
    switch (Index)
    {
    case 0:
        return SpawnProjectileToTarget(OwnerComp, BossPtr);
    case 1:
        return SpawnProjectilesStraight(OwnerComp, BossPtr);
    case 2:
        return SpawnFromFront(OwnerComp, BossPtr);
    case 3:
        return SpawnCircleStraight(OwnerComp, BossPtr);
    case 4:
        return SpawnGangplankUlt(OwnerComp, BossPtr);
    default:
        break;
    }
    return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_FireStraightProjectile::SpawnProjectileToTarget(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
    if (BossPtr == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    AActor* PawnActorPtr = Cast<AActor>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
    if (PawnActorPtr == nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("PawnActorPtr Empty"));
        return EBTNodeResult::Failed;
    }
    FVector Direction = PawnActorPtr->GetActorLocation() - BossPtr->GetActorLocation();
    Direction.Normalize();

    FVector SpawnLocation = BossPtr->GetActorLocation() + Direction * 300.0f;
    FRotator Rot(Direction.Rotation());

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
    
    
    AVMStraightProjectile* SpawnActor = SpawnStraightProjectile(GetWorld(), SpawnLocation, Rot, Params);
    if (SpawnActor == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    SpawnActor->SetMaxSpeed(3000);
    SpawnActor->SetVelocity(3000);

    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_FireStraightProjectile::SpawnProjectilesStraight(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
    if (BossPtr == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ShotsFired = 0;

    // 첫 발사 즉시 실행
    FireOneVerticalProjectile(&OwnerComp, BossPtr);

    // 나머지는 타이머로 실행
    BossPtr->GetWorldTimerManager().SetTimer(
        VerticalShotTimer,
        [this, &OwnerComp, BossPtr]()
        {
            FireOneVerticalProjectile(&OwnerComp, BossPtr);
        },
        1.0f,
        true
    );

    // 비동기 실행이므로 즉시 성공 반환
    return EBTNodeResult::InProgress;
}

void UBTTask_FireStraightProjectile::FireOneVerticalProjectile(UBehaviorTreeComponent* OwnerComp, AVMEnemyBoss* BossPtr)
{
    if (!BossPtr)
        return;

    const float PitchStart = -50.f;
    const float PitchEnd = -5.f;

    float PitchStep = (PitchEnd - PitchStart) / (TotalShots - 1);
    float Pitch = PitchStart + PitchStep * ShotsFired;

    FVector SpawnLocation = BossPtr->GetActorLocation() + BossPtr->GetActorForwardVector() * 300.f;
    FRotator Rot(Pitch, BossPtr->GetActorRotation().Yaw, 0.f);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
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
    
    // 실제 발사
    SpawnStraightProjectile(BossPtr->GetWorld(), SpawnLocation, Rot, Params);

    ShotsFired++;

    // 모두 쏘면 타이머 종료 + BT 완료 처리
    if (ShotsFired >= TotalShots)
    {
        BossPtr->GetWorldTimerManager().ClearTimer(VerticalShotTimer);

        OwnerComp->OnTaskFinished(
            this,
            EBTNodeResult::Succeeded
        );
    }
}



EBTNodeResult::Type UBTTask_FireStraightProjectile::SpawnFromFront(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
    if (!BossPtr)
        return EBTNodeResult::Failed;

    const int32 Count = 20;
    const float Radius = 4000.f;
    FVector BossLoc = BossPtr->GetActorLocation();
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

    for (int32 i = 0; i < Count; i++)
    {
        // 원형 각도 계산
        float YawAngle = (360.f / Count) * i;
        FRotator SpawnRot(0.f, YawAngle, 0.f);

        // 보스 기준 원형 위치
        FVector SpawnLocation = BossLoc + SpawnRot.Vector() * Radius;

        // 발사체가 보스를 향하도록 회전
        FRotator Rot = (BossLoc - SpawnLocation).Rotation();
        SpawnLocation.Z = BossLoc.Z - 200.f;

        
        
        AVMStraightProjectile* SpawnActor = SpawnStraightProjectile(GetWorld(), SpawnLocation, Rot, Params);
        if (SpawnActor == nullptr)
        {
            EBTNodeResult::Failed;
        }
        SpawnActor->SetMaxSpeed(2000);
        SpawnActor->SetVelocity(2000);
    }

    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_FireStraightProjectile::SpawnCircleStraight(
    UBehaviorTreeComponent& OwnerComp,
    AVMEnemyBoss* BossPtr)
{
    if (!BossPtr)
        return EBTNodeResult::Failed;

    ShotsFired = 0;

    // 첫 발사 즉시 실행
    FireOneCircleProjectile(&OwnerComp, BossPtr);

    // 타이머로 1초 간격 발사
    BossPtr->GetWorldTimerManager().SetTimer(
        CircleShotTimer,
        [this, &OwnerComp, BossPtr]()
        {
            FireOneCircleProjectile(&OwnerComp, BossPtr);
        },
        0.2f, // 1초 간격
        true
    );

    return EBTNodeResult::InProgress; // 비동기 실행
}

void UBTTask_FireStraightProjectile::FireOneCircleProjectile(UBehaviorTreeComponent* OwnerComp, AVMEnemyBoss* BossPtr)
{
    if (!BossPtr) return;

    const float AngleStep = 360.f / CircleTotalShots;
    const float Distance = 400.0f;
    FVector BaseLoc = BossPtr->GetActorLocation();
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

    float Yaw = ShotsFired * AngleStep;
    FRotator Rot(0.f, Yaw, 0.f);
    FVector Loc = BaseLoc + Rot.Vector() * Distance;
    Loc.Z -= 200;

    SpawnStraightProjectile(GetWorld(), Loc, Rot, Params);

    ShotsFired++;

    if (ShotsFired >= CircleTotalShots)
    {
        // 모두 쏘면 타이머 종료 + BT 완료
        BossPtr->GetWorldTimerManager().ClearTimer(CircleShotTimer);
        OwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);
    }
}

EBTNodeResult::Type UBTTask_FireStraightProjectile::SpawnGangplankUlt(UBehaviorTreeComponent& OwnerComp,AVMEnemyBoss* BossPtr)
{
    if (!BossPtr)
        return EBTNodeResult::Failed;

    UWorld* World = BossPtr->GetWorld();
    if (!World)
        return EBTNodeResult::Failed;

    // 초기화
    ShotsFired = 0;
    //TotalShots = 30;

    // 첫 발사 즉시 실행
    FireOneVerticalProjectile(&OwnerComp, BossPtr);

    // 나머지는 타이머로 순차 발사
    World->GetTimerManager().SetTimer(
        VerticalShotTimer,
        [this, &OwnerComp, BossPtr, World]()
        {
            if (!BossPtr || !World)
            {
                World->GetTimerManager().ClearTimer(VerticalShotTimer);
                return;
            }

            // 매 발사 시점에 랜덤 X,Y 생성
            FVector BossLoc = BossPtr->GetActorLocation();
            const float Radius = 2000.f;
            float RandRadius = FMath::Sqrt(FMath::FRand()) * Radius;
            float RandAngle = FMath::FRandRange(0.f, 360.f);
            float Rad = FMath::DegreesToRadians(RandAngle);
            FVector SpawnPos = FVector(
                BossLoc.X + FMath::Cos(Rad) * RandRadius,
                BossLoc.Y + FMath::Sin(Rad) * RandRadius,
                BossLoc.Z + 3000.f
            );

            FRotator Rot = FRotator(-90.f, 0.f, 0.f);

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
            
            // 발사
            SpawnStraightProjectile(World, SpawnPos, Rot, Params);

            ShotsFired++;

            // 마지막 발사 후 종료
            if (ShotsFired >= CircleTotalShots)
            {
                World->GetTimerManager().ClearTimer(VerticalShotTimer);
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }

        },
        0.3f,
        true
    );

    return EBTNodeResult::InProgress;
}

AVMStraightProjectile* UBTTask_FireStraightProjectile::SpawnStraightProjectile(UWorld* World, FVector SpawnLocation, FRotator SpawnRotation, const FActorSpawnParameters& Params)
{
	return World->SpawnActor<AVMStraightProjectile>(AVMStraightProjectile::StaticClass(), SpawnLocation, SpawnRotation, Params);
}
