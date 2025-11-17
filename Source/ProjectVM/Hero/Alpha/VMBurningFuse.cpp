// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMBurningFuse.h"

UVMBurningFuse::UVMBurningFuse(const FObjectInitializer& ObjectInitializer)
{
}

void UVMBurningFuse::ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp)
{
	Super::ActivateSkill(InOwner, StatComp);
	UE_LOG(LogTemp, Log, TEXT("스킬 : BurningFuse"));
}
