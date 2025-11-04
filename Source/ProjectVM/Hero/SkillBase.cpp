// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/SkillBase.h"
#include "Hero/HeroStat.h"

void USkillBase::ActivateSkill(FHeroStat& CurStat)
{
	CurStat.ManaPoint -= ManaCost;
	RemainingCooldown = Cooldown;
}

bool USkillBase::IsSkillValid(FHeroStat CurStat)
{
	if (CurStat.ManaPoint < ManaCost) return false;
	if (RemainingCooldown > KINDA_SMALL_NUMBER) return false;
	
	return true;
}

void USkillBase::ReduceCooldown(float DeltaTime)
{
	if (RemainingCooldown <= KINDA_SMALL_NUMBER) return;

	RemainingCooldown = FMath::Max<float>(RemainingCooldown - DeltaTime, 0.0f);
	if (RemainingCooldown <= KINDA_SMALL_NUMBER) RemainingCooldown = 0.0f;
}