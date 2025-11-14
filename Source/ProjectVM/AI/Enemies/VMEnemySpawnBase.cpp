// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMEnemySpawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Quest/VMQuestManager.h"

#include "Macro/VMPhysics.h"

// Sets default values
AVMEnemySpawnBase::AVMEnemySpawnBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(VM_ENEMY_COLLISION);

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionProfileName(NOCOLLISION);

	InitDefaultAssetSetting();
}


// Called when the game starts or when spawned
void AVMEnemySpawnBase::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add("Enemy");
}

// Called every frame
void AVMEnemySpawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVMEnemySpawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVMEnemySpawnBase::HealthPointChange(float Amount, AActor* Causer)
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemySpawnBase::HealthPointChange Damage:%f Causer: %s"), Amount, *Causer->GetName());

	SetCurrentHp(FMath::Clamp<float>(GetCurrentHp() - Amount, 0, GetMaxHp()));

	if (GetCurrentHp() < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Log, TEXT("몬스터가 죽었습니다"));
		GetGameInstance()->GetSubsystem<UVMQuestManager>()->NotifyMonsterDeath(GetMonsterType());
		Destroy();
	}
}

void AVMEnemySpawnBase::OnHealHp(float HealGauge)
{
	CurrentHp = FMath::Clamp<float>(CurrentHp + HealGauge, 0, MaxHp);
}

void AVMEnemySpawnBase::InitDefaultAssetSetting()
{
	// Skeletal Mesh 입히기
#pragma region SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Buff/Buff_Blue/Meshes/Buff_Blue.Buff_Blue'"));
	ensureAlways(MeshRef.Object);
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
#pragma endregion 
}

