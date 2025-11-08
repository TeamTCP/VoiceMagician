// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMTitlePlayerController.h"

AVMTitlePlayerController::AVMTitlePlayerController()
{
}

void AVMTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
