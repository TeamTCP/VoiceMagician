// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVMGameMode.h"
#include "ProjectVMCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectVMGameMode::AProjectVMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
