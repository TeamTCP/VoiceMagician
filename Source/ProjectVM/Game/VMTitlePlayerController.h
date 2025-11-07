// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VMTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVMTitlePlayerController();

protected:
	virtual void BeginPlay() override;
	
};
