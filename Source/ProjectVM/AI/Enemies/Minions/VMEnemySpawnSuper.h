// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enemies/VMEnemySpawnBase.h"
#include "VMEnemySpawnSuper.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMEnemySpawnSuper : public AVMEnemySpawnBase
{
	GENERATED_BODY()

public:
	AVMEnemySpawnSuper();
public:
	virtual void BeginPlay() override;
public:
	FORCEINLINE float GetAIMoveSpeed() { return MoveSpeed; }
	FORCEINLINE float GetAIAttackSpeed() { return AttackSpeed; }
	FORCEINLINE float GetAIAttackRange() { return AttackRange; }
	FORCEINLINE float GetAITurnSpeed() { return TurnSpeed; }

private:
	float MoveSpeed = 200.0f;
	float AttackSpeed = 1.0f;
	float AttackRange = 200.0f;
	float TurnSpeed = 2.0f;
};
