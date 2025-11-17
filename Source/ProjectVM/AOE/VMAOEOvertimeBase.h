// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AOE/VMAOEBase.h"
#include "VMAOEOvertimeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMAOEOvertimeBase : public AVMAOEBase
{
	GENERATED_BODY()

public:
	AVMAOEOvertimeBase();

	virtual void BeginPlay() override;

	virtual void TriggerSpawnAOESphere() override;

	// 부모에서 이미 했음.
	virtual void SpawnAOESphere() override;

	void DestroyWrapper();


	FTimerHandle TimeHandle;
	FTimerHandle FinishTimeHandle;
};
