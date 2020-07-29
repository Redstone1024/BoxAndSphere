// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedPointMath.h"
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
	FORCEINLINE FFixedVector(FFixed InF) : X(InF), Y(InF), Z(InF) { }
	FORCEINLINE FFixedVector(FFixed InX, FFixed InY, FFixed InZ) : X(InX), Y(InY), Z(InZ) { }
	explicit FORCEINLINE FFixedVector(const FVector& InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }
	explicit FORCEINLINE operator FVector() const { return FVector(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z)); }
	explicit FORCEINLINE FFixedVector(const FIntVector& InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }
	explicit FORCEINLINE operator FIntVector() const { return FIntVector(static_cast<int32>(X), static_cast<int32>(Y), static_cast<int32>(Z)); }

	FORCEINLINE FFixedVector& operator ^=(const FFixedVector& V) { *this = *this ^ V; return *this; }
	FORCEINLINE FFixedVector operator ^(const FFixedVector& V) const { return FFixedVector(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X); }
	FORCEINLINE FFixed operator |(const FFixedVector& V) const { return X * V.X + Y * V.Y + Z * V.Z; }

#define FIXED_NAS_OP(O) friend FORCEINLINE FFixedVector operator O(FFixedVector A, FFixedVector B) { A O##= B; return A; }
#define FIXED_BIN_OP(O) FORCEINLINE FFixedVector &operator O ##=(const FFixedVector& V) { X O##= V.X; Y O##= V.Y; Z O##= V.Z; return *this; } FIXED_NAS_OP(O)

	FIXED_BIN_OP(+)
	FIXED_BIN_OP(-)
	FIXED_BIN_OP(*)
	FIXED_BIN_OP(/)

#undef FIXED_BIN_OP
#undef FIXED_NAS_OP

	FORCEINLINE FFixedVector operator -() const { return FFixedVector(-X, -Y, -Z); }
	FORCEINLINE bool operator ==(const FFixedVector& V) const { return (X == V.X) && (Y == V.Y) && (Z == V.Z); }
	FORCEINLINE bool operator !=(const FFixedVector& V) const { return (X != V.X) || (Y != V.Y) || (Z != V.Z); }

	FORCEINLINE FFixed& operator [](int32 Index) { check(Index >= 0 && Index < 3); return (&X)[Index]; }
	FORCEINLINE FFixed operator [](int32 Index) const { check(Index >= 0 && Index < 3); return (&X)[Index]; }

	FORCEINLINE bool IsZero() const { return X == 0 && Y == 0 && Z == 0; }
	FORCEINLINE FFixed Length() const { return FFixedMath::Sqrt(LengthSquared()); }
	FORCEINLINE FFixed LengthSquared() const { return X * X + Y * Y + Z * Z; }
	FORCEINLINE bool Normalize();
};

namespace FFixedMath
{
	// 常用数学

	FORCEINLINE FFixedVector LerpVector(FFixedVector A, FFixedVector B, FFixed Alpha) { return FMath::Lerp(A, B, Alpha); }
	FORCEINLINE FFixedVector ComponentAbs(const FFixedVector& A) { return FFixedVector(FFixedMath::Abs(A.X), FFixedMath::Abs(A.Y), FFixedMath::Abs(A.Z)); }
	FORCEINLINE FFixedVector ComponentMin(const FFixedVector& A, const FFixedVector& B) { return FFixedVector(FFixedMath::Min(A.X, B.X), FFixedMath::Min(A.Y, B.Y), FFixedMath::Min(A.Z, B.Z)); }
	FORCEINLINE FFixedVector ComponentMax(const FFixedVector& A, const FFixedVector& B) { return FFixedVector(FFixedMath::Max(A.X, B.X), FFixedMath::Max(A.Y, B.Y), FFixedMath::Max(A.Z, B.Z)); }

	// 向量运算

	FORCEINLINE FFixedVector CrossProduct(const FFixedVector& A, const FFixedVector& B) { return A ^ B; }
	FORCEINLINE FFixed DotProduct(const FFixedVector& A, const FFixedVector& B) { return A | B; }
	FORCEINLINE FFixedVector Normalize(const FFixedVector& A) { FFixedVector Result = A; Result.Normalize(); return Result; }
}

FORCEINLINE bool FFixedVector::Normalize()
{
	FFixed SquareSum = LengthSquared();

	if (SquareSum < 0)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("Vector length is too long. (An overflow may have occurred!)"));
		return false;
	}

	FFixed Length = FFixedMath::Sqrt(SquareSum);
	if (Length > 0) {
		*this /= Length;
		return true;
	}

	return false;
}