// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEOvertimeBase.h"

AVMAOEOvertimeBase::AVMAOEOvertimeBase()
{

}

void AVMAOEOvertimeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AVMAOEOvertimeBase::TriggerSpawnAOESphere()
{
	Super::TriggerSpawnAOESphere();

	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AVMAOEOvertimeBase::SpawnAOESphere, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(FinishTimeHandle, this, &AVMAOEOvertimeBase::DestroyWrapper, 3.0f, false);
}

void AVMAOEOvertimeBase::SpawnAOESphere()
{
	Super::SpawnAOESphere();
}

void AVMAOEOvertimeBase::DestroyWrapper()
{
	Destroy();
}
