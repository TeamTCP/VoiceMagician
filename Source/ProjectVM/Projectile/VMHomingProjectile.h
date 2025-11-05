// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMHomingProjectile.generated.h"

UCLASS()
class PROJECTVM_API AVMHomingProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMHomingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBoxComponent> Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UArrowComponent> Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> Particle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystem> HitParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr <class AActor> Target;
	
	void EnableCollision();

public:
	void SetTarget(AActor* NewTarget);



public:
	FComponentHitSignature HitSignature;

	UFUNCTION()
	void HitAndDestroy(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
