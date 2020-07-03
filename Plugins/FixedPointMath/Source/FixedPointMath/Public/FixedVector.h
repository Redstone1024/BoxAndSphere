// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedMath.h"
#include "FixedVector.generated.h"

USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Vector", SaveGame)
	FFixed X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Vector", SaveGame)
	FFixed Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Vector", SaveGame)
	FFixed Z;

	static const FFixedVector ZeroVector;
	static const FFixedVector OneVector;
	static const FFixedVector UpVector;
	static const FFixedVector DownVector;
	static const FFixedVector ForwardVector;
	static const FFixedVector BackwardVector;
	static const FFixedVector RightVector;
	static const FFixedVector LeftVector;

	FORCEINLINE FFixedVector() { }

	explicit FORCEINLINE FFixedVector(FFixed InF) : X(InF), Y(InF), Z(InF) { }

	FORCEINLINE FFixedVector(FFixed InX, FFixed InY, FFixed InZ) : X(InX), Y(InY), Z(InZ) { }

	explicit FORCEINLINE FFixedVector(const FVector& InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }

	explicit FORCEINLINE operator FVector() const { return FVector(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z)); }

	explicit FORCEINLINE FFixedVector(const FIntVector& InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }

	explicit FORCEINLINE operator FIntVector() const { return FIntVector(static_cast<int32>(X), static_cast<int32>(Y), static_cast<int32>(Z)); }

	FORCEINLINE FFixedVector& operator ^=(const FFixedVector& RHS) { *this = *this ^ RHS; return *this; }

	FORCEINLINE FFixedVector operator ^(const FFixedVector& RHS) const { return FFixedVector(Y * RHS.Z - Z * RHS.Y, Z * RHS.X - X * RHS.Z, X * RHS.Y - Y * RHS.X); }

	FORCEINLINE FFixed operator |(const FFixedVector& RHS) const { return X * RHS.X + Y * RHS.Y + Z * RHS.Z; }

#define FIXED_BIN_OP(O) FORCEINLINE FFixedVector &operator O ##=(const FFixedVector &RHS) { X O##= RHS.X; Y O##= RHS.Y; Z O##= RHS.Z; return *this; } \
        FORCEINLINE FFixedVector operator O(const FFixedVector &RHS) const { FFixedVector Temp(*this); Temp O##= RHS; return Temp; }

	FIXED_BIN_OP(+)
	FIXED_BIN_OP(-)
	FIXED_BIN_OP(*)
	FIXED_BIN_OP(/)

#undef FIXED_BIN_OP

	FORCEINLINE FFixedVector operator -() const { return FFixedVector(-X, -Y, -Z); }

	FORCEINLINE bool operator ==(const FFixedVector& RHS) const { return (X == RHS.X) && (Y == RHS.Y) && (Z == RHS.Z); }

	FORCEINLINE bool operator !=(const FFixedVector& RHS) const { return (X != RHS.X) || (Y != RHS.Y) || (Z != RHS.Z); }

	FORCEINLINE bool IsZero() const { return X == 0 && Y == 0 && Z == 0; }

	FORCEINLINE FFixed Length() const { return FFixedMath::Sqrt(X * X + Y * Y + Z * Z); }

	FORCEINLINE FFixed LengthSquared() const { return X * X + Y * Y + Z * Z; }

	FORCEINLINE FFixedVector Normalize();

};

FORCEINLINE FFixedVector FFixedVector::Normalize()
{
	FFixed SquareSum = LengthSquared();

	if (SquareSum < 0)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("Vector length is too long. (An overflow may have occurred!)"));
		return FFixedVector::ZeroVector;
	}

	if (SquareSum > 0)
		*this /= FFixedVector(Length());

	return *this;
}