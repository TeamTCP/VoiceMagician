// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Alpha.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AAlpha : public AVMCharacterHeroBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
