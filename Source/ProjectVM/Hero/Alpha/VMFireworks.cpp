// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMFireworks.h"

UVMFireworks::UVMFireworks(const FObjectInitializer& ObjectInitializer)
{
}

void UVMFireworks::ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp)
{
	Super::ActivateSkill(InOwner, StatComp);
	UE_LOG(LogTemp, Log, TEXT("스킬 : Fireworks"));
}
