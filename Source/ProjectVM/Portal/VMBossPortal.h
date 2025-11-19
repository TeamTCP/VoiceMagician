// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/VMPortal.h"
#include "VMBossPortal.generated.h"

UCLASS()
class PROJECTVM_API AVMBossPortal : public AVMPortal
{
	GENERATED_BODY()

public:
	AVMBossPortal();
protected:
	virtual void Interact() override;
	void LoadMap();

	//UFUNCTION()
	//void OnBossLevelLoaded();

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	//FVector MapTeleportPos = FVector(12000.f, 12000.f, 0.f);
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	//FVector PlayerTeleportPos = FVector(-1800.f, -3000.f, 500.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetMap")
	FString TargetLevelPath = "/Game/Project/Map/TestBossMap_Environment.TestBossMap_Environment";
};
