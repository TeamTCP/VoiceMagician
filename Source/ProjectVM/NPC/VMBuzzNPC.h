// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/VMNPC.h"
#include "VMBuzzNPC.generated.h"


UCLASS()
class PROJECTVM_API AVMBuzzNPC : public AVMNPC
{
	GENERATED_BODY()
	
public:
	AVMBuzzNPC();

	virtual void EnterShop() override;

protected:
	virtual void SetDialogueOption() override;

};
