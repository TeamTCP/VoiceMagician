// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WarriorSkill1.h"

#include "AIController.h"
#include "VMEnemyBase.h"

UBTTask_WarriorSkill1::UBTTask_WarriorSkill1()
{
	NodeName = TEXT("Skill1");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_WarriorSkill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("WarriorSkill One"));

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AIController is nullptr"));
		return EBTNodeResult::Failed;
	}
	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Pawn is nullptr"));
		return EBTNodeResult::Failed;
	}

	AVMEnemyBase* EnemyPtr = Cast<AVMEnemyBase>(Pawn);
	if (EnemyPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase is nullptr"));
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = EnemyPtr->GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(EnemyPtr->SkillAttack1Montage);
		bIsAttacking = true;
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_WarriorSkill1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	ElapsedTime += DeltaSeconds;

	AAIController* AIController = OwnerComp.GetAIOwner();

	AVMEnemyBase* Character = Cast<AVMEnemyBase>(AIController->GetPawn());
	if (!Character)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 3️⃣ 공격 종료 조건 판정
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(Character->SkillAttack1Montage))
	{
		UE_LOG(LogTemp, Log, TEXT("Finish"));

		// 애니메이션이 끝났으면 종료
		bIsAttacking = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
