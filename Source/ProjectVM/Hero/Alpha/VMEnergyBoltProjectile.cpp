// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMEnergyBoltProjectile.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"

#include "Interface/VMStatChangeable.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Utils/VMMath.h"


AVMEnergyBoltProjectile::AVMEnergyBoltProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	RootComponent = SplinePath;
	SplinePath->bDrawDebug = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(1.f);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVMEnergyBoltProjectile::HitTarget);
	
	EnergyBoltEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EnergyBoltEffect"));
	EnergyBoltEffect->SetupAttachment(SphereCollision);
	EnergyBoltEffect->SetAutoActivate(true);
	EnergyBoltEffect->SetAutoDestroy(true);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/_SplineVFX/NS/NS_Spline_EnergyLoop.NS_Spline_EnergyLoop"));
	if (NiagaraSystemAsset.Succeeded())
	{
		EnergyBoltEffect->SetAsset(NiagaraSystemAsset.Object);
		
		EnergyBoltEffect->SetVariableFloat(TEXT("_ColorHue"), 0.5f);
		EnergyBoltEffect->SetVariableFloat(TEXT("_Size"), 3.0f);
		EnergyBoltEffect->SetVariableFloat(TEXT("_Speed"), 4.0f);
		EnergyBoltEffect->SetVariableFloat(TEXT("Count"), 100.0f);
		EnergyBoltEffect->SetVariableFloat(TEXT("Extent"), 10.0f);
		EnergyBoltEffect->SetVariableFloat(TEXT("Noise"), 99.0f);
		EnergyBoltEffect->SetVariableFloat(TEXT("Progress"), 1.0f);
		
		EnergyBoltEffect->SetVariableBool(TEXT("AddDetail"), false);
		EnergyBoltEffect->SetVariableBool(TEXT("LightON"), false);
		EnergyBoltEffect->SetVariableBool(TEXT("withHead"), false);
	}
	
	Progress = 0.0f;
}

void AVMEnergyBoltProjectile::BindOwnerAndTarget(AActor* InOwner, AActor* InTarget)
{
	if (InOwner == nullptr || InOwner->IsValidLowLevel() == false)
	{
		return;
	}
	
	if (InTarget == nullptr || InTarget->IsValidLowLevel() == false)
	{
		return;
	}
	
	SetOwner(InOwner);
	Target = InTarget;

	SplinePath->ClearSplinePoints();

	FVector StartLocation = InOwner->GetActorLocation();
	FVector EndLocation = InTarget->GetActorLocation();

	FVector CurveLocation = StartLocation + (EndLocation - StartLocation) * FMath::FRandRange(0.1f, 0.3f);
	float RandomRadius = FMath::FRandRange(100.0f, 200.0f);
	float RandomTheta = FMath::FRandRange(0.0f, PI);

	FCircle3D Circle(CurveLocation, EndLocation - StartLocation, RandomRadius);
	CurveLocation = Circle.GetLocation(RandomTheta);

	// DrawDebugCircle(GetWorld(), Circle.Center, Circle.Radius, 32, FColor::Green, false, 3.f, 0, 1.f, Circle.AxisX, Circle.AxisY, true);
	
	SplinePath->AddSplinePoint(StartLocation, ESplineCoordinateSpace::World);
	SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);
	
	SplinePath->AddSplinePoint(CurveLocation, ESplineCoordinateSpace::World);
	SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);
	
	SplinePath->AddSplinePoint(EndLocation, ESplineCoordinateSpace::World);
	SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Linear);
	
	SplinePath->UpdateSpline();
}

void AVMEnergyBoltProjectile::HitTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != Target) return;
	
	IVMStatChangeable* StatChangeable = Cast<IVMStatChangeable>(OtherActor);

	if (StatChangeable)
	{
		UE_LOG(LogTemp, Log, TEXT("타겟 %s 충돌 !"), *OtherActor->GetName());
		StatChangeable->HealthPointChange(10.f, this);
	}

	SphereCollision->SetActive(false);
}

void AVMEnergyBoltProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AVMEnergyBoltProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr)
	{
		Destroy();
		return;
	}

	Progress += DeltaTime * 2;
	float SplineLength = SplinePath->GetSplineLength();
	float TargetLength = SplineLength * Progress;
	
	FVector Pos = SplinePath->GetLocationAtDistanceAlongSpline(TargetLength, ESplineCoordinateSpace::World);
	SphereCollision->SetWorldLocation(Pos);

	EnergyBoltEffect->SetVariableVec3(TEXT("CollisionPos"), Pos);
	
	if (Progress > 3.0f) Destroy();
}