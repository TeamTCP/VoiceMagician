// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_UpdateAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/VMAIEnemyBaseInterface.h"

UBTService_UpdateAttackRange::UBTService_UpdateAttackRange()
{
}

void UBTService_UpdateAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// AIController가 제어하는 폰 가져오기.
	AAIController* ControllerPtr = OwnerComp.GetAIOwner();
	if (ControllerPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateAttackRange nullptr1"));
		return;
	}

	APawn* PawnPtr = ControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateAttackRange nullptr2"));
		return;
	}

	UBlackboardComponent* BBCompPtr = OwnerComp.GetBlackboardComponent();
	if (BBCompPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateAttackRange nullptr3"));
		return;
	}

	APawn* EnemyPawnPtr = Cast<APawn>(BBCompPtr->GetValueAsObject(TEXT("EnemyTarget")));
	if (EnemyPawnPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateAttackRange nullptr4"));
		return;
	}
	
	IVMAIEnemyBaseInterface* AIEnemyBaseInterfacePtr = Cast<IVMAIEnemyBaseInterface>(PawnPtr);
	if (AIEnemyBaseInterfacePtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateAttackRange nullptr5"));
		return;
	}


	float AttackRange = AIEnemyBaseInterfacePtr->GetAIAttackRange();
	BBCompPtr->SetValueAsFloat(TEXT("AttackRange"), AttackRange);
	if (EnemyPawnPtr->GetDistanceTo(PawnPtr) <= AttackRange)
	{
		BBCompPtr->SetValueAsBool(TEXT("bCanAttack"), true);
	}
	else
	{
		BBCompPtr->SetValueAsBool(TEXT("bCanAttack"), false);
	}
	bool bCanAttack = BBCompPtr->GetValueAsBool(TEXT("bCanAttack"));
	//UE_LOG(LogTemp, Log, TEXT("UBTService_UpdateAttackRange %f %d"), AttackRange, bCanAttack);
}
