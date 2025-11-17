// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMFlamingLaunch.h"

UVMFlamingLaunch::UVMFlamingLaunch(const FObjectInitializer& ObjectInitializer)
{
}

void UVMFlamingLaunch::ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp)
{
	Super::ActivateSkill(InOwner, StatComp);
	UE_LOG(LogTemp, Log, TEXT("스킬 : FlamingLaunch"));
}
