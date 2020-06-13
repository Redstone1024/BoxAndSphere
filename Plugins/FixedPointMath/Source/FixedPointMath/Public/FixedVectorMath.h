// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FixedMath.h"
#include "FixedVector.h"
#include "FixedPointMath.h"

class FIXEDPOINTMATH_API FFixedVectorMath
{
public:

	// 精度转换

	template<int64 DPR, int64 DPP>
	static FORCEINLINE TFixedVector<DPR> PrecConv(const TFixedVector<DPP>& A) 
	{ return TFixedVector<DPR>(FFixedMath::PrecConv(A.X), FFixedMath::PrecConv(A.Y), FFixedMath::PrecConv(A.Z)); }

	// 常用数学

	template<int64 DP>
	static FORCEINLINE TFixedVector<DP> ComponentAbs(const TFixedVector<DP>& A) 
	{ return TFixedVector<DP>(FFixedMath::Abs(A.X), FFixedMath::Abs(A.Y), FFixedMath::Abs(A.Z)); }

	template<int64 DP>
	static FORCEINLINE TFixedVector<DP> ComponentMin(const TFixedVector<DP>& A, const TFixedVector<DP>& B) 
	{ return TFixedVector<DP>(FFixedMath::Min(A.X, B.X), FFixedMath::Min(A.Y, B.Y), FFixedMath::Min(A.Z, B.Z)); }

	template<int64 DP>
	static FORCEINLINE TFixedVector<DP> ComponentMax(const TFixedVector<DP>& A, const TFixedVector<DP>& B) 
	{ return TFixedVector<DP>(FFixedMath::Max(A.X, B.X), FFixedMath::Max(A.Y, B.Y), FFixedMath::Max(A.Z, B.Z)); }

	// 向量运算

	template<int64 DP>
	static FORCEINLINE TFixedVector<DP> CrossProduct(const TFixedVector<DP>& A, const TFixedVector<DP>& B) { return A ^ B; }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> DotProduct(const TFixedVector<DP>& A, const TFixedVector<DP>& B) { return A | B; }

	template<int64 DP>
	static FORCEINLINE TFixedVector<DP> Normalize(const TFixedVector<DP>& A) { TFixedVector<DP> Result = A; return Result.Normalize(); }
};
