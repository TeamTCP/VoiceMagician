// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEBomb.h"

// Sets default values
AVMAOEBomb::AVMAOEBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVMAOEBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVMAOEBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

