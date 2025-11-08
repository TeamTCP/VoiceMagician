// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGGameMode.h"
#include "Game/VMRPGPlayerController.h"
#include "Hero/Alpha/Alpha.h"

AVMRPGGameMode::AVMRPGGameMode()
{
	PlayerControllerClass = AVMRPGPlayerController::StaticClass();
	DefaultPawnClass = AAlpha::StaticClass();
}
