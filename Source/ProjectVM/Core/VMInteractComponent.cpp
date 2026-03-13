// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/VMInteractComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Core/VMInteractionManager.h"
#include "Engine/GameInstance.h"

void UVMInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UVMInteractComponent::OnInteractTriggerOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UVMInteractComponent::OnInteractTriggerOverlapEnd);

	// 루트 컴포넌트 가져오기
	USceneComponent* RootComp = GetOwner()->GetRootComponent();
	if (!RootComp) return;

	// 루트 컴포넌트의 로컬 바운드 가져오기
	FBoxSphereBounds RootBounds = RootComp->CalcBounds(RootComp->GetComponentTransform());

	// BoxExtent 설정 (약간 여유 포함)
	FVector Margin(50.f, 50.f, 50.f); // 필요에 따라 조정
	SetBoxExtent(RootBounds.BoxExtent + Margin);

	// 박스 위치 루트에 맞추기
	SetRelativeLocation(FVector::ZeroVector);
}

void UVMInteractComponent::OnInteractTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(OtherActor);
	if(Player == nullptr)
	{
		return;
	}

	UVMInteractionManager* InteractionManager = GetOwner()->GetGameInstance()->GetSubsystem<UVMInteractionManager>();

	if (InteractionManager == nullptr)
	{
		return;
	}

	//상호작용 매니저에 등록
	InteractionManager->RegisterInteractable(GetOwner());
}

void UVMInteractComponent::OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}

	UVMInteractionManager* InteractionManager = GetOwner()->GetGameInstance()->GetSubsystem<UVMInteractionManager>();

	if (InteractionManager == nullptr)
	{
		return;
	}

	//상호작용 매니저에서 해제
	InteractionManager->UnregisterInteractable(GetOwner());
}