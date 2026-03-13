// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/VMStraightProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

#include "Hero/VMCharacterHeroBase.h"

// Sets default values
AVMStraightProjectile::AVMStraightProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Box 콜라이더 생성 및 초기화
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collider->SetBoxExtent(FVector(50, 25, 25));
	Collider->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// 루트 컴포넌트 지정
	RootComponent = Collider;

	// Mesh 생성 및 BoxCollision에 붙임.
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Mesh->SetupAttachment(Collider);

	// Arrow 생성 및 BoxCollision에 붙임.
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Collider);
	Arrow->SetRelativeLocation(FVector(50, 0, 0));

	// Arrow 생성 및 BoxCollision에 붙임.
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(Collider);
	Particle->SetRelativeLocation(FVector(30, 0, 0));

	// ProjectileMovement 생성. Actor 컴포넌트이므로 붙일 필요 없음.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

#pragma region 에셋로드
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_Projectile.P_Gideon_Burden_Projectile'"));
	ensureAlways(ParticleRef.Object);
	if (ParticleRef.Object)
	{
		Particle->SetTemplate(ParticleRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld'"));
	ensureAlways(HitParticleRef.Object);
	if (HitParticleRef.Object)
	{
		HitParticle = HitParticleRef.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> HitSoundRef(TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	ensureAlways(HitSoundRef.Object);
	if (HitSoundRef.Object)
	{
		HitSound = HitSoundRef.Object;
	}
#pragma endregion

	MaxSpeed = InitialSpeed = 500.0f;

	// 투사체 속도 설정.
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;

	// 중력 설정.
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AVMStraightProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;

	if (Collider)
	{
		Collider->OnComponentHit.AddDynamic(this, &AVMStraightProjectile::HitAndDestroy);
	}
}

void AVMStraightProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMStraightProjectile::HitAndDestroy(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 이펙트 재생
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), false);

	// 소리 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.ImpactPoint);
	

	AVMCharacterHeroBase* HeroPawn = Cast<AVMCharacterHeroBase>(OtherActor);
	// TODO: 데미지 주는 거 필요. 일단 Base에 넣는걸로 하자.
	if (HeroPawn == nullptr)
	{
		Destroy();
		return;
	}

	HeroPawn->HealthPointChange(1, OtherActor);

	// 객체 파괴
	Destroy();
}

void AVMStraightProjectile::SetMaxSpeed(float InMaxSpeed)
{ 
	ProjectileMovement->MaxSpeed = MaxSpeed = InMaxSpeed;
}

void AVMStraightProjectile::SetVelocity(float InWeight)
{
	ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * InWeight;
}
