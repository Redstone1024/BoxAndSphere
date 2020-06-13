// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedPointMath.h"

class FIXEDPOINTMATH_API FFixedMath
{
public:

	// 精度转换

	template<int64 DPR, int64 DPP>
	static FORCEINLINE TFixed<DPR> PrecConv(TFixed<DPP> A);

	// 常用数学

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Abs(TFixed<DP> A) { return FMath::Abs(A); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Min(TFixed<DP> A, TFixed<DP> B) { return FMath::Min(A, B); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Max(TFixed<DP> A, TFixed<DP> B) { return FMath::Max(A, B); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Clamp(TFixed<DP> Value, TFixed<DP> Min, TFixed<DP> Max) { return FMath::Clamp(Value, Min, Max); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Sqrt(TFixed<DP> A);

	// 三角函数

	static TFixed<4096> TanUnitRaw(TFixed<4096> A);

	static TFixed<4096> SinUnitRaw(TFixed<4096> A);

	static TFixed<4096> CosUnitRaw(TFixed<4096> A);

	template<int64 DP>
	static FORCEINLINE TFixed<DP> TanDeg(TFixed<DP> A) { return TanUnit<DP>(A / TFixed<DP>(360)); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> SinDeg(TFixed<DP> A) { return SinUnit<DP>(A / TFixed<DP>(360)); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> CosDeg(TFixed<DP> A) { return CosUnit<DP>(A / TFixed<DP>(360)); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Tan(TFixed<DP> A) { return TanUnit<DP>(A / (TFixed<DP>(2) * PrecConv<DP>(GetPI<DP>()))); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Sin(TFixed<DP> A) { return SinUnit<DP>(A / (TFixed<DP>(2) * PrecConv<DP>(GetPI<DP>()))); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> Cos(TFixed<DP> A) { return CosUnit<DP>(A / (TFixed<DP>(2) * PrecConv<DP>(GetPI<DP>()))); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> GetPI() { TFixed<268435456> Temp; Temp.Data = 843314857; return FFixedMath::PrecConv<DP>(Temp); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> TanUnit(TFixed<DP> A) { return FFixedMath::PrecConv<DP>(FFixedMath::TanUnitRaw(FFixedMath::PrecConv<4096>(A))); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> SinUnit(TFixed<DP> A) { return FFixedMath::PrecConv<DP>(FFixedMath::SinUnitRaw(FFixedMath::PrecConv<4096>(A))); }

	template<int64 DP>
	static FORCEINLINE TFixed<DP> CosUnit(TFixed<DP> A) { return FFixedMath::PrecConv<DP>(FFixedMath::CosUnitRaw(FFixedMath::PrecConv<4096>(A))); }

};

template<int64 DPR, int64 DPP>
FORCEINLINE TFixed<DPR> FFixedMath::PrecConv(TFixed<DPP> A)
{
	TFixed<DPR> Temp;
	Temp = static_cast<TFixed<DPR>>(A.Data);
	Temp.Data /= TFixed<DPP>::DecimalPrecision;
	return Temp;
}

template<int64 DP>
FORCEINLINE TFixed<DP> FFixedMath::Sqrt(TFixed<DP> A)
{
	if (A < TFixed<DP>::Zero)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("The square root cannot be negative. (An overflow may have occurred!)"));
		return TFixed<DP>(0);
	}

	// TODO: 将此处的二分求解换成更快的算法

	TFixed<DP> Temp;

	if (A == TFixed<DP>::Zero) return TFixed<DP>::Zero;
	else
	{
		uint64 t;
		uint64 q = 0;
		uint64 r = A;
		uint64 b = 0x4000000000000000;
		while (b > 0)
		{
			t = q + b;
			q >>= 1;
			if (r >= t)
			{
				r -= t;
				q += b;
			}
			b >>= 2;
		}
		Temp = TFixed<DP>(q);

		TFixed<DP> L = Temp;
		TFixed<DP> R = Temp + TFixed<DP>::One;
		TFixed<DP> M = (L + R) / TFixed<DP>(2);
		while (L < R)
		{
			if (M * M < A) L = M + TFixed<DP>::Unit;
			else R = M - TFixed<DP>::Unit;
			M = (L + R) / TFixed<DP>(2);
		}
		Temp = L;
	}

	return Temp;
}
