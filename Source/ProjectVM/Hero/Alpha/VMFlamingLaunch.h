// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/SkillBase.h"
#include "VMFlamingLaunch.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMFlamingLaunch : public USkillBase
{
	GENERATED_BODY()

public:
	UVMFlamingLaunch(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp) override;
};
