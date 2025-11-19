// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/VMBossPortal.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


AVMBossPortal::AVMBossPortal()
{
	MapTeleportPos = FVector(12000.f, 12000.f, 0.f);
	PlayerTeleportPos = FVector(-1800.f, -3000.f, 500.f);
}

void AVMBossPortal::Interact()
{
	Super::Interact();

	LoadMap();
}

void AVMBossPortal::LoadMap()
{
	////포탈로 이동하는 이펙트 라인 숨기기
	//if (EffectLineNiagaraSystem == nullptr)
	//{
	//	return;
	//}
	//EffectLineNiagaraSystem->Deactivate(); // 비활성화

	//맵 이동
	if (!GetWorld()) return;

	bool bSuccess = false;

	ULevelStreamingDynamic* LoadedLevel = ULevelStreamingDynamic::LoadLevelInstance(
		GetWorld(),
		TargetLevelPath,      // FString 레벨 경로
		MapTeleportPos,  // 위치
		FRotator::ZeroRotator,// 회전
		bSuccess
	);

	if (bSuccess && LoadedLevel)
	{
		LoadedLevel->SetShouldBeLoaded(true);
		LoadedLevel->SetShouldBeVisible(true);
		LoadedLevel->OnLevelLoaded.AddDynamic(this, &AVMPortal::TeleportPlayerToMap); //레벨이 로드가 완료되면 실행하는 함수 바인딩
	}
}

