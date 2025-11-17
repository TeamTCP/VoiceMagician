// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/SkillBase.h"
#include "VMBurningFuse.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMBurningFuse : public USkillBase
{
	GENERATED_BODY()

public:
	UVMBurningFuse(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp) override;
};
