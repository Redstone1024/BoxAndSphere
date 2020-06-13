// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedMath.h"

template<int64 DP>
struct FIXEDPOINTMATH_API TFixedVector
{
public:

	TFixed<DP> X;

	TFixed<DP> Y;

	TFixed<DP> Z;

	static const TFixedVector ZeroVector;
	static const TFixedVector OneVector;
	static const TFixedVector UpVector;
	static const TFixedVector DownVector;
	static const TFixedVector ForwardVector;
	static const TFixedVector BackwardVector;
	static const TFixedVector RightVector;
	static const TFixedVector LeftVector;

	FORCEINLINE TFixedVector() { }

	explicit FORCEINLINE TFixedVector(TFixed<DP> InF) : X(InF), Y(InF), Z(InF) { }

	FORCEINLINE TFixedVector(TFixed<DP> InX, TFixed<DP> InY, TFixed<DP> InZ) : X(InX), Y(InY), Z(InZ) { }

	explicit FORCEINLINE TFixedVector(const FVector& InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }

	explicit FORCEINLINE operator FVector() const { return FVector(X, Y, Z); }

	explicit FORCEINLINE TFixedVector(const FIntVector& InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }

	explicit FORCEINLINE operator FIntVector() const { return FIntVector(static_cast<int32>(X), static_cast<int32>(Y), static_cast<int32>(Z)); }

	FORCEINLINE TFixedVector& operator ^=(const TFixedVector& RHS) { *this = *this ^ RHS; return *this; }

	FORCEINLINE TFixedVector operator ^(const TFixedVector& RHS) const { return TFixedVector(Y * RHS.Z - Z * RHS.Y, Z * RHS.X - X * RHS.Z, X * RHS.Y - Y * RHS.X); }

	FORCEINLINE TFixed<DP> operator |(const TFixedVector& RHS) const { return X * RHS.X + Y * RHS.Y + Z * RHS.Z; }

#define FIXED_BIN_OP(O) FORCEINLINE TFixedVector &operator O ##=(const TFixedVector &RHS) { X O##= RHS.X; Y O##= RHS.Y; Z O##= RHS.Z; return *this; } \
        FORCEINLINE TFixedVector operator O(const TFixedVector &RHS) const { TFixedVector Temp(*this); Temp O##= RHS; return Temp; }

	FIXED_BIN_OP(+)
	FIXED_BIN_OP(-)
	FIXED_BIN_OP(*)
	FIXED_BIN_OP(/)

#undef FIXED_BIN_OP

	FORCEINLINE TFixedVector operator -() const { return TFixedVector(-X, -Y, -Z); }

	FORCEINLINE bool operator ==(const TFixedVector &RHS) const { return (X == RHS.X) && (Y == RHS.Y) && (Z == RHS.Z); }

	FORCEINLINE bool operator !=(const TFixedVector &RHS) const { return (X != RHS.X) || (Y != RHS.Y) || (Z != RHS.Z); }

	FORCEINLINE bool IsZero() const { return X == TFixed<DP>::Zero && Y == TFixed<DP>::Zero && Z == TFixed<DP>::Zero; }

	FORCEINLINE TFixed<DP> Length() const { return FFixedMath::PrecConv<DP>(FFixedMath::Sqrt(FFixedMath::PrecConv<65536>(X * X + Y * Y + Z * Z))); }

	FORCEINLINE TFixed<DP> LengthSquared() const { return X * X + Y * Y + Z * Z; }

	FORCEINLINE TFixedVector<DP> Normalize();

};

typedef TFixedVector<FFixed::DecimalPrecision> FFixedVector;

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::ZeroVector = TFixedVector<DP>(TFixed<DP>::Zero);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::OneVector = TFixedVector<DP>(TFixed<DP>::One);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::UpVector = TFixedVector<DP>(0, 0, 1);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::DownVector = TFixedVector<DP>(0, 0, -1);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::ForwardVector = TFixedVector<DP>(1, 0, 0);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::BackwardVector = TFixedVector<DP>(-1, 0, 0);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::RightVector = TFixedVector<DP>(0, 1, 0);

template<int64 DP>
const TFixedVector<DP> TFixedVector<DP>::LeftVector = TFixedVector<DP>(0, -1, 0);

template<int64 DP>
FORCEINLINE TFixedVector<DP> TFixedVector<DP>::Normalize()
{
	TFixed<DP> SquareSum = LengthSquared();

	if (SquareSum < TFixed<DP>::Zero)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("Vector length is too long. (An overflow may have occurred!)"));
		return TFixedVector<DP>::ZeroVector;
	}

	if (SquareSum > TFixed<DP>::Zero)
		*this /= TFixedVector<DP>(Length());

	return *this;
}
