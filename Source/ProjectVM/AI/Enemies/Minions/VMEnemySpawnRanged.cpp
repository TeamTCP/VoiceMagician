// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/Minions/VMEnemySpawnRanged.h"

#include "Core/VMLevelManager.h"
#include "Projectile/VMStraightProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AVMEnemySpawnRanged::AVMEnemySpawnRanged()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonMinions/Characters/Minions/Down_Minions/Animations/Melee/ABP_SpawnEnemy.ABP_SpawnEnemy_C'"));
	ensureAlways(AnimInstanceRef.Class);
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimClass(AnimInstanceRef.Class);
	}

	// Skeletal Mesh 입히기
#pragma region SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Ranged_Dusk.Minion_Lane_Ranged_Dusk'"));
	ensureAlways(MeshRef.Object);
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
#pragma endregion

#pragma region AnimationClass
	ConstructorHelpers::FClassFinder<AController> AIControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVM.VMSpawnAIControllerBase'"));
	ensureAlways(AIControllerClassRef.Class);
	if (AIControllerClassRef.Class)
	{
		AIControllerClass = AIControllerClassRef.Class;
	}
#pragma endregion

	ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/VM_RangeAttackDefault.VM_RangeAttackDefault'"));
	if (NormalAttackMontageRef.Object)
	{
		NormalAttackMontage = NormalAttackMontageRef.Object;
	}

	MoveSpeed = 200.0f;
	AttackSpeed = 1.0f;
	AttackRange = 1000.0f;
	TurnSpeed = 2.0f;
}

void AVMEnemySpawnRanged::BeginPlay()
{
	Super::BeginPlay();
	SetMonsterType(EMonsterName::MinionRanged);
}

void AVMEnemySpawnRanged::NormalAttack()
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemySpawnRanged::NormalAttack"));
	Super::NormalAttack();

	FVector Location = GetActorLocation();
	FVector SpawnLocation = Location + GetActorForwardVector() * 100.0f;
	FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

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
	
	
	AVMStraightProjectile* Projectile = GetWorld()->SpawnActor<AVMStraightProjectile>(AVMStraightProjectile::StaticClass(), SpawnTransform, SpawnParams);
	if (Projectile == nullptr)
	{
		return;
	}

	Projectile->SetMaxSpeed(1000.0f);
	Projectile->SetVelocity(1000.0f);

	UE_LOG(LogTemp, Display, TEXT("Ranged가 발사한 Projectile을 생성하였습니다."));
}
