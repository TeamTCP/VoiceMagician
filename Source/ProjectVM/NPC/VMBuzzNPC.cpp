#include "VMBuzzNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "GameData/VMQuestData.h"
#include "Core/GameEnums.h"
#include "Game/VMRPGPlayerController.h"

AVMBuzzNPC::AVMBuzzNPC()
{
	NPCType = ENPCType::Buzz;

	GetCapsuleComponent()->InitCapsuleSize(150.f, 250.0f);
	// ½ºÄÌ·¹Å» ¸Þ½Ã ¼³Á¤
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("/Game/NPCContent/Buzz/Buzz.Buzz"));
	if (NPCMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(NPCMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(-85.0f, 0.0f, -280.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	// ABP ¼³Á¤
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPCAnimBP(TEXT("/Game/Project/NPC/ABP_Bazz.ABP_Bazz_C"));
	if (NPCAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPCAnimBP.Class);
	}

	InteractKey->SetWorldLocation(FVector(40.0f, 0.0f, -20.0f));
	InteractKeyBoxComponent->SetBoxExtent(FVector(200.0f, 200.0f, 320.0f));
}

void AVMBuzzNPC::EnterShop()
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}

	PC->ShowScreen(EScreenUIType::ShopScreen);
}

void AVMBuzzNPC::SetDialogueOption()
{
	AddDialogueOption(ENPCOption::Talk);
	if (!AvailableQuests.IsEmpty())
	{
		AddDialogueOption(ENPCOption::Quest);
	}
	if (!CompletedQuests.IsEmpty())
	{
		AddDialogueOption(ENPCOption::QuestClear);
	}
	AddDialogueOption(ENPCOption::Shop);
	AddDialogueOption(ENPCOption::Exit);
}
