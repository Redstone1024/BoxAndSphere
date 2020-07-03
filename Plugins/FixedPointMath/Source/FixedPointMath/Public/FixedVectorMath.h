// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FixedMath.h"
#include "FixedVector.h"

class FIXEDPOINTMATH_API FFixedVectorMath
{
public:
	// 常用数学

	static FORCEINLINE FFixedVector ComponentAbs(const FFixedVector& A)
	{ return FFixedVector(FFixedMath::Abs(A.X), FFixedMath::Abs(A.Y), FFixedMath::Abs(A.Z)); }

	static FORCEINLINE FFixedVector ComponentMin(const FFixedVector& A, const FFixedVector& B) 
	{ return FFixedVector(FFixedMath::Min(A.X, B.X), FFixedMath::Min(A.Y, B.Y), FFixedMath::Min(A.Z, B.Z)); }

	static FORCEINLINE FFixedVector ComponentMax(const FFixedVector& A, const FFixedVector& B) 
	{ return FFixedVector(FFixedMath::Max(A.X, B.X), FFixedMath::Max(A.Y, B.Y), FFixedMath::Max(A.Z, B.Z)); }

	// 向量运算

	static FORCEINLINE FFixedVector CrossProduct(const FFixedVector& A, const FFixedVector& B) { return A ^ B; }
	static FORCEINLINE FFixed DotProduct(const FFixedVector& A, const FFixedVector& B) { return A | B; }
	static FORCEINLINE FFixedVector Normalize(const FFixedVector& A) { FFixedVector Result = A; return Result.Normalize(); }
};
