// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/Alpha.h"
#include "Hero/Alpha/EnergyBolt.h"
#include "Hero/VMHeroSkillComponent.h"

void AAlpha::BeginPlay()
{
	Super::BeginPlay();

	USkillBase* EnergyBolt = NewObject<UEnergyBolt>(this, UEnergyBolt::StaticClass());
	Skills->BindBasicSkill(EnergyBolt);
}
