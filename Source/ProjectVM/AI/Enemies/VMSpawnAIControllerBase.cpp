// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMSpawnAIControllerBase.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Macro/VMAIMarco.h"
#include "GameFramework/Character.h"

AVMSpawnAIControllerBase::AVMSpawnAIControllerBase()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBWAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/EnemySpawn/BB_VMEnemySpawnBase.BB_VMEnemySpawnBase'"));
	ensureAlways(BBWAssetRef.Object);
	
	// Blackboard 에셋 연결
	if (BBWAssetRef.Object)
	{
		BBAsset = BBWAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTWAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/EnemySpawn/BT_VMEnemySpawnBase.BT_VMEnemySpawnBase'"));
	ensureAlways(BTWAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTWAssetRef.Object)
	{
		BTAsset = BTWAssetRef.Object;
		BTAsset->BlackboardAsset = BBAsset;
	}
}

void AVMSpawnAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Log, TEXT("BeginPlay Name: %s"), *GetName());
	
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Name: %s BeginPlay"), *PlayerCharacter->GetName());
		UBlackboardComponent* BBComp = GetBlackboardComponent();
		if (BBComp)
		{
			BBComp->SetValueAsObject(TEXT("EnemyTarget"), PlayerCharacter);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Name: null BeginPlay"));
	}
	

	 RunBehaviorTree(BTAsset);
}

void AVMSpawnAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Name: %s OnPossess"), *PlayerCharacter->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Name: null OnPossess"));
	}

	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	/*if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Name: nullptr인데"));
	}*/
	if (PlayerCharacter)
	{
		UBlackboardComponent* BBComp = GetBlackboardComponent();
		if (BBComp)
		{
			BBComp->SetValueAsObject(TEXT("EnemyTarget"), PlayerCharacter);
		}
	}


	RunAI();
}

void AVMSpawnAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}


void AVMSpawnAIControllerBase::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	bool UseResult = UseBlackboard(BBAsset, BlackboardPtr);
	if (UseResult == false)
	{
		ensureAlways(UseResult);
		return;
	}

	bool RunResult = RunBehaviorTree(BTAsset);
	if (RunResult == false)
	{
		ensureAlways(RunResult);
		return;
	}
}

// AI를 중지하는 함수.
void AVMSpawnAIControllerBase::StopAI()
{
	// 실행 중인 BehaviorTreeComponent 가져오기.
	UBehaviorTreeComponent* BTComponentPtr = Cast<UBehaviorTreeComponent>(BrainComponent);

	// BT 실행 중지.
	if (BTComponentPtr == nullptr)
	{
		return;
	}

	BTComponentPtr->StopTree();
}