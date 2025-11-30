// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEMeteor.h"
#include "AOE/VMAOEFire.h"

#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Core/VMLevelManager.h"
#include "Materials/MaterialInterface.h"

#include "Particles/ParticleSystem.h"

#include "Engine/OverlapResult.h"

#include "Hero/VMCharacterHeroBase.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Macro/VMPhysics.h"



AVMAOEMeteor::AVMAOEMeteor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComp->SetupAttachment(RootComponent);
    DecalComp->SetHiddenInGame(true);

    ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalCompMeterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/KTP_Decal/Decal/plant_DID_101125.plant_DID_101125'"));
    if (DecalCompMeterialRef.Object)
    {
        DecalComp->SetDecalMaterial(DecalCompMeterialRef.Object);
    }
}

void AVMAOEMeteor::BeginPlay()
{
    Super::BeginPlay();
    CreateLogic();
    /*GetWorld()->GetTimerManager().SetTimer(DecalTimeHandle, [this]()
        {
            CreateLogic();
        }, 3.0f, false);*/
}

void AVMAOEMeteor::CreateLogic()
{
    DecalComp->DecalSize = FVector(256.f, 256.f, 256.f);
    DecalComp->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
    DecalComp->MarkRenderStateDirty(); // 갱신 강제 굳이?
    DecalComp->SetHiddenInGame(false);


    //DecalMaterial, Size, Location, Rotation, 0
    DecalTimeHandle.Invalidate();

    GetWorld()->GetTimerManager().SetTimer(DecalTimeHandle, [this]()
        {
            UE_LOG(LogTemp, Log, TEXT("Hello"));
            if (DecalComp == nullptr)
            {
                UE_LOG(LogTemp, Log, TEXT("No Nullptr"));
            }
            if (IsValid(DecalComp))
            {
                UE_LOG(LogTemp, Log, TEXT("정상이니까 삭제"));
                DecalComp->DestroyComponent();
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("비정상이니까 삭제 안함."));
            }
            InitAOEPosition();
            SpawnAOE();
        }, 3.0f, false);
}

void AVMAOEMeteor::InitAOEPosition()
{
    // z축으로 Lay를 쏴서 부딪힌 Mesh의 좌표를 얻는다.
    DecalLocation = GetActorLocation();
    FVector StartLocation = DecalLocation + FVector(0, 0, 1000);
    FVector EndLocation = DecalLocation + FVector(0, 0, -1000);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);    // 자기 자신 무시

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params);
    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            FloorLocation = Hit.Location;
        }
    }
    FColor LineColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 2.0f, 0, 2.0f);
}

void AVMAOEMeteor::SpawnAOE()
{
    // 사운드 동기 로드
    USoundBase* MySound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
    if (MySound == nullptr)
    {
        return;
    }

    // 사운드 재생.
    UGameplayStatics::PlaySoundAtLocation(this, MySound, FloorLocation, 1.0f, 1.0f);

    // 이펙트 동기 로드
    UParticleSystem* ParticleSystem = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
    if (ParticleSystem == nullptr)
    {
        return;
    }

    // 이펙트 재생.
    UGameplayStatics::SpawnEmitterAttached(ParticleSystem, RootComponent, TEXT("NoName"), FVector::Zero(), GetActorRotation(), EAttachLocation::KeepRelativeOffset);


    // Sphere를 그리고 충돌 처리를 하는게 나을 듯?
    // 충돌 채널 설정
    TArray<FOverlapResult> Overlaps;
    float Radius = 256.0f;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
    bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, FloorLocation, FQuat::Identity, VM_HERO, Sphere);
    if (bHasOverlap)
    {
        for (auto& Result : Overlaps)
        {
            AActor* OverlappedActor = Result.GetActor();
            if (OverlappedActor)
            {
                
                BroadcastOverlapActor(OverlappedActor, 10);
            }
        }
    }
    
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    //Spawn Level BossMap으로 한정. BossMap 없으면 퍼시스턴트 레벨에 소환
    UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();
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
    
    AVMAOEFire* FireSpawnActor = GetWorld()->SpawnActor<AVMAOEFire>(AVMAOEFire::StaticClass(), GetActorTransform(), Params);
    if (FireSpawnActor == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("운석 충돌 -> 불 소환 실패"));
    }
}

void AVMAOEMeteor::BroadcastOverlapActor(AActor* Actor, float InDamage)
{
    UE_LOG(LogTemp, Log, TEXT("AVMAOEMeteor::BroadcastOverlapActor"));
    OnAOEMeteorOverlapActor.Broadcast(Actor, InDamage);
}
