// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FCircle3D
{
	FVector Center;
	FVector Normal;
	float Radius;

	FVector AxisX;
	FVector AxisY;

	FCircle3D(const FVector& InCenter, const FVector& InAxisX, const FVector& InAxisY, const float InRadius = 1.0f) : Center(InCenter), Radius(InRadius), AxisX(InAxisX), AxisY(InAxisY)
	{
		Normal = FVector::CrossProduct(InAxisX, InAxisY);
	}
	
	FCircle3D(const FVector& InCenter, const FVector& InNormal, const float InRadius = 1.0f) : Center(InCenter), Normal(InNormal.GetSafeNormal()), Radius(InRadius)
	{
		if (1.0f - FMath::Abs(FVector::DotProduct(Normal, FVector::UpVector)) < KINDA_SMALL_NUMBER)
		{
			// Y축과 평행한 AxisX 생성
			AxisX = FVector::RightVector;
			AxisY = FVector::CrossProduct(Normal, AxisX).GetSafeNormal();
		}
		else
		{
			// XY 평면과 평행한 AxisX 생성
			AxisX = FVector::CrossProduct(FVector::UpVector, Normal).GetSafeNormal();
			AxisY = FVector::CrossProduct(Normal, AxisX).GetSafeNormal();
		}
	}

	FORCEINLINE FVector GetLocation(float Theta) const
	{
		return Center + Radius * (AxisX * FMath::Cos(Theta) + AxisY * FMath::Sin(Theta));
	}
};

class PROJECTVM_API VMMath
{
public:
	VMMath() = delete;
	~VMMath() = delete;
};
