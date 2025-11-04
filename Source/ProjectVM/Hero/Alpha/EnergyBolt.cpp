// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/EnergyBolt.h"
#include "Hero/HeroStat.h"

UEnergyBolt::UEnergyBolt(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("EnergyBolt");
	SkillDesc = TEXT("에너지 볼트를 발사합니다.");
	ManaCost = 10;
	Cooldown = 5;
	RemainingCooldown = 0;
}

void UEnergyBolt::ActivateSkill(FHeroStat& CurStat)
{
	Super::ActivateSkill(CurStat);

	UE_LOG(LogTemp, Log, TEXT("Skill : EnergyBolt !"));
}