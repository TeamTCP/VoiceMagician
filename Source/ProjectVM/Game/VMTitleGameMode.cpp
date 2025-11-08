// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMTitleGameMode.h"
#include "Game/VMTitlePlayerController.h"

AVMTitleGameMode::AVMTitleGameMode()
{
	PlayerControllerClass = AVMTitlePlayerController::StaticClass();
}
