// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"

class FIXEDPOINTMATH_API FFixedMath
{
public:

	// 精度转换

	template<size_t DPR, size_t DPP>
	static FORCEINLINE TFixed<DPR> PrecConv(TFixed<DPP> A)
	{
		if (TFixed<DPR>::DecimalPrecision == TFixed<DPP>::DecimalPrecision)
			return A;

		TFixed<DPR> Temp;
		Temp = static_cast<TFixed<DPR>>(A.Data);
		Temp.Data /= TFixed<DPP>::DecimalPrecision;
		return Temp;
	}

	// 常用数学

	template<size_t DP>
	static FORCEINLINE TFixed<DP> Min(TFixed<DP> A, TFixed<DP> B) { return FMath::Min(A, B); }

	template<size_t DP>
	static FORCEINLINE TFixed<DP> Max(TFixed<DP> A, TFixed<DP> B) { return FMath::Max(A, B); }

	template<size_t DP>
	static FORCEINLINE TFixed<DP> Clamp(TFixed<DP> Value, TFixed<DP> Min, TFixed<DP> Max) { return FMath::Clamp(Value, Min, Max); }

	// 三角函数 - 注意！0~1 表示 0~360 度！

	static TFixed<4096> Tan(TFixed<4096> A);

	static TFixed<4096> Sin(TFixed<4096> A);

	static TFixed<4096> Cos(TFixed<4096> A);
};
