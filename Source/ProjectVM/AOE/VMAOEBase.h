// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOEBase.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnAOEOverlapActorDelegate, AActor* /*Target*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAOEOverlapActorDelegate, AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAOEExplosionOverlapActorDelegate, AActor*, Target, FVector, ExplosionCenter);

UCLASS()
class PROJECTVM_API AVMAOEBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOEBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION()
	virtual void SpawnAOESphere();

	UFUNCTION()
	virtual void TriggerSpawnAOESphere();

	/*UFUNCTION()
	void PlaySound();

	UFUNCTION()
	void OnOverlapAOEActor(class AActor* Actor);*/

public:	// 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "AOE Event")
	FOnAOEOverlapActorDelegate OnAOEOverlapActor;

	UPROPERTY(BlueprintAssignable, Category = "AOE Event")
	FOnAOEExplosionOverlapActorDelegate OnAOEExplosionOverlapActor;

	UFUNCTION()
	void FOnAOEOverlapActor(AActor* Target);

	UFUNCTION()
	void FOnAOEOverlapActorWithCenter(AActor* Target, FVector ExplosionCenter);

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRootComp;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	//TObjectPtr<class USphereComponent> SphereCollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member)
	uint8 bDrawDebugSphere : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member)
	uint8 bIgnoreInstigator : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member)
	FColor Color;

public:
	FORCEINLINE void SetRadius(float InRadius) { Radius = InRadius; }
};
