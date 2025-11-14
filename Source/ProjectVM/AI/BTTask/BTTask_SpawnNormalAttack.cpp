// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SpawnNormalAttack.h"

#include "AIController.h"
#include "AI/VMEnemyBase.h"

UBTTask_SpawnNormalAttack::UBTTask_SpawnNormalAttack()
{
	bNotifyTick = true;
}


EBTNodeResult::Type UBTTask_SpawnNormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVMEnemyBase* EnemyPtr = Cast<AVMEnemyBase>(PawnPtr);
	if (EnemyPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = EnemyPtr->GetMesh()->GetAnimInstance();
	ensureAlways(AnimInstance);
	if (AnimInstance)
	{
		const float AnimPlayRate = 2.0f;
		AnimInstance->Montage_Play(EnemyPtr->LaserAttackMontage, AnimPlayRate);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_SpawnNormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	AVMEnemyBase* Character = Cast<AVMEnemyBase>(AIController->GetPawn());
	if (!Character)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 3️⃣ 공격 종료 조건 판정
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(Character->LaserAttackMontage))
	{
		UE_LOG(LogTemp, Log, TEXT("Finish"));

		// 애니메이션이 끝났으면 종료
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}