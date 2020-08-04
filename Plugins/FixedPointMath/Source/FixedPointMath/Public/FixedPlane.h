// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedVector.h"
#include "FixedPointMath.h"
#include "FixedPlane.generated.h"

USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedPlane
	: public FFixedVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Plane", SaveGame)
	FFixed W;

	FORCEINLINE FFixedPlane() { }
	FORCEINLINE FFixedPlane(FFixed InX, FFixed InY, FFixed InZ, FFixed InW) : FFixedVector(InX, InY, InZ), W(InW) { }
	FORCEINLINE FFixedPlane(const FFixedVector& InNormal, FFixed InW) : FFixedVector(InNormal), W(InW) { }
	FORCEINLINE FFixedPlane(const FFixedVector& InBase, const FFixedVector &InNormal) : FFixedVector(InNormal), W(InBase | InNormal) { }
	FORCEINLINE FFixedPlane(FFixedVector A, FFixedVector B, FFixedVector C) : FFixedVector(FFixedMath::Normalize((B - A) ^ (C - A))), W(A | (FFixedVector)(*this)) { }

	explicit FORCEINLINE FFixedPlane(const FPlane& InPlane) : FFixedVector(InPlane), W(InPlane.W) { }
	explicit FORCEINLINE operator FPlane() const { return FPlane(static_cast<FVector>(*this), static_cast<float>(W)); }

	FORCEINLINE FFixed PlaneDot(const FFixedVector &P) const { return X * P.X + Y * P.Y + Z * P.Z - W; }
	FORCEINLINE bool Normalize();

	FORCEINLINE FFixedPlane operator -() const { return FFixedPlane(-X, -Y, -Z, -W); }
	FORCEINLINE bool operator ==(const FFixedPlane& V) const { return (X == V.X) && (Y == V.Y) && (Z == V.Z) && (W == V.W); }
	FORCEINLINE bool operator !=(const FFixedPlane& V) const { return (X != V.X) || (Y != V.Y) || (Z != V.Z) || (W != V.W); }
	FORCEINLINE FFixed operator |(const FFixedPlane& V) const { return X * V.X + Y * V.Y + Z * V.Z + W * V.W; }
	friend FORCEINLINE FFixedPlane operator +(const FFixedPlane& A, const FFixedPlane& B) { return FFixedPlane(A.X + B.X, A.Y + B.Y, A.Z + B.Z, A.W + B.W); }
	friend FORCEINLINE FFixedPlane operator -(const FFixedPlane& A, const FFixedPlane& B) { return FFixedPlane(A.X - B.X, A.Y - B.Y, A.Z - B.Z, A.W - B.W); }
	friend FORCEINLINE FFixedPlane operator *(const FFixedPlane& A, const FFixedPlane& B) { return FFixedPlane(A.X * B.X, A.Y * B.Y, A.Z * B.Z, A.W * B.W); }
	friend FORCEINLINE FFixedPlane operator /(const FFixedPlane& A, const FFixedPlane& B) { return FFixedPlane(A.X / B.X, A.Y / B.Y, A.Z / B.Z, A.W / B.W); }
	FORCEINLINE FFixedPlane operator /(FFixed Scale) const { return FFixedPlane(X / Scale, Y / Scale, Z / Scale, W / Scale); }
	FORCEINLINE FFixedPlane operator *(FFixed Scale) const { return FFixedPlane(X * Scale, Y * Scale, Z * Scale, W * Scale); }
	FORCEINLINE FFixedPlane& operator +=(const FFixedPlane& V) { X += V.X; Y += V.Y; Z += V.Z; W += V.W; return *this; }
	FORCEINLINE FFixedPlane& operator -=(const FFixedPlane& V) { X -= V.X; Y -= V.Y; Z -= V.Z; W -= V.W; return *this; }
	FORCEINLINE FFixedPlane& operator *=(const FFixedPlane& V) { X *= V.X; Y *= V.Y; Z *= V.Z; W *= V.W; return *this; }
	FORCEINLINE FFixedPlane& operator /=(const FFixedPlane& V) { X /= V.X; Y /= V.Y; Z /= V.Z; W /= V.W; return *this; }
	FORCEINLINE FFixedPlane& operator *=(FFixed Scale) { X *= Scale; Y *= Scale; Z *= Scale; W *= Scale; return *this; }
	FORCEINLINE FFixedPlane& operator /=(FFixed Scale) { X /= Scale; Y /= Scale; Z /= Scale; W /= Scale; return *this; }

	FORCEINLINE FFixed& operator [](int32 Index) { check(Index >= 0 && Index < 4); return (&X)[Index]; }
	FORCEINLINE FFixed operator [](int32 Index) const { check(Index >= 0 && Index < 4); return (&X)[Index]; }
};

FORCEINLINE bool FFixedPlane::Normalize()
{
	FFixed SquareSum = LengthSquared();

	if (SquareSum < 0)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("Vector length is too long. (An overflow may have occurred!)"));
		return false;
	}

	FFixed Length = FFixedMath::Sqrt(SquareSum);

	if (Length > 0)
	{
		*this /= Length;
		return true;
	}

	return false;
}
