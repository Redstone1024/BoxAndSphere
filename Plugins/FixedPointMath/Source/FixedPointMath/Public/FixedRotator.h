// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedVector.h"
#include "FixedPointMath.h"
#include "FixedRotator.generated.h"

USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedRotator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Rotator", SaveGame, meta = (DisplayName = "Y"))
	FFixed Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Rotator", SaveGame, meta = (DisplayName = "Z"))
	FFixed Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Rotator", SaveGame, meta = (DisplayName = "X"))
	FFixed Roll;

	static const FFixedRotator ZeroRotator;
	
	FORCEINLINE FFixedRotator() { }
	explicit FORCEINLINE FFixedRotator(FFixed InF) : Pitch(InF), Yaw(InF), Roll(InF) { }
	FORCEINLINE FFixedRotator(FFixed InPitch, FFixed InYaw, FFixed InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) { }
	explicit FORCEINLINE FFixedRotator(const FRotator& InRotator) : Pitch(InRotator.Pitch), Yaw(InRotator.Yaw), Roll(InRotator.Roll) { }
	explicit FORCEINLINE operator FRotator() const { return FRotator(static_cast<float>(Pitch), static_cast<float>(Yaw), static_cast<float>(Roll)); }

	FORCEINLINE FFixedRotator &operator +=(const FFixedRotator& R) { Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll; return *this; }
	friend FORCEINLINE FFixedRotator operator +(FFixedRotator A, FFixedRotator B) { A += B; return A; }
	FORCEINLINE FFixedRotator &operator -=(const FFixedRotator& R) { Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll; return *this; }
	friend FORCEINLINE FFixedRotator operator -(FFixedRotator A, FFixedRotator B) { A -= B; return A; }
	FORCEINLINE FFixedRotator &operator *=(const FFixed Scale) { Pitch *= Scale; Yaw *= Scale; Roll *= Scale; return *this; }
	FORCEINLINE FFixedRotator operator *(const FFixed Scale) const { FFixedRotator Temp(*this); Temp *= Scale; return Temp; }
	FORCEINLINE FFixedRotator &operator /=(const FFixed Scale) { Pitch /= Scale; Yaw /= Scale; Roll /= Scale; return *this; }
	FORCEINLINE FFixedRotator operator /(const FFixed Scale) const { FFixedRotator Temp(*this); Temp /= Scale; return Temp; }

	FORCEINLINE FFixedRotator operator -() const { return FFixedRotator(-Pitch, -Yaw, -Roll); }
	FORCEINLINE bool operator ==(const FFixedRotator& R) const { return (Pitch == R.Pitch) && (Yaw == R.Yaw) && (Roll == R.Roll); }
	FORCEINLINE bool operator !=(const FFixedRotator& R) const { return (Pitch != R.Pitch) || (Yaw != R.Yaw) || (Roll != R.Roll); }

	FORCEINLINE bool IsZero() const { return Pitch == 0 && Yaw == 0 && Roll == 0; }
};

namespace FFixedMath
{
	FORCEINLINE FFixed ClampAxis(FFixed Angle);
	FORCEINLINE FFixedRotator ClampRotator(FFixedRotator R) { return FFixedRotator(ClampAxis(R.Pitch), ClampAxis(R.Yaw), ClampAxis(R.Roll)); }
	FORCEINLINE FFixedRotator LerpRotator(FFixedRotator A, FFixedRotator B, FFixed Alpha) { return A + (B - A) * Alpha; }
}

FORCEINLINE FFixed FFixedMath::ClampAxis(FFixed Angle)
{
	Angle = Angle % 360;
	if (Angle < 0) Angle += 360;
	return Angle;
}
