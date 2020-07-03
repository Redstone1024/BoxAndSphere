// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedPointMath.h"

class FIXEDPOINTMATH_API FFixedMath
{
public:

	// 常用数学

	static FORCEINLINE FFixed Abs(FFixed A) { return FMath::Abs(A); }
	static FORCEINLINE FFixed Min(FFixed A, FFixed B) { return FMath::Min(A, B); }
	static FORCEINLINE FFixed Max(FFixed A, FFixed B) { return FMath::Max(A, B); }
	static FORCEINLINE FFixed Clamp(FFixed Value, FFixed Min, FFixed Max) { return FMath::Clamp(Value, Min, Max); }
	static FORCEINLINE FFixed Sqrt(FFixed A);

	// 三角函数

	static FFixed TanUnit(FFixed A);
	static FFixed SinUnit(FFixed A);
	static FFixed CosUnit(FFixed A);
	static FORCEINLINE FFixed TanDeg(FFixed A) { return TanUnit(A / FFixed(360)); }
	static FORCEINLINE FFixed SinDeg(FFixed A) { return SinUnit(A / FFixed(360)); }
	static FORCEINLINE FFixed CosDeg(FFixed A) { return CosUnit(A / FFixed(360)); }
	static FORCEINLINE FFixed Tan(FFixed A) { return TanUnit(A / (FFixed(2) * FFixed::Pi)); }
	static FORCEINLINE FFixed Sin(FFixed A) { return SinUnit(A / (FFixed(2) * FFixed::Pi)); }
	static FORCEINLINE FFixed Cos(FFixed A) { return CosUnit(A / (FFixed(2) * FFixed::Pi)); }

};

FORCEINLINE FFixed FFixedMath::Sqrt(FFixed A)
{
	if (A < 0)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("The square root cannot be negative. (An overflow may have occurred!)"));
		return 0;
	}

	// TODO: 将此处的二分求解换成更快的算法

	FFixed Temp;

	if (A == 0) return 0;
	else
	{
		uint64 t;
		uint64 q = 0;
		uint64 r = (uint64)A;
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
		Temp = FFixed(q);

		FFixed L = Temp;
		FFixed R = Temp + 1;
		FFixed M = (L + R) >> 1;
		while (L < R)
		{
			if (M * M < A) L = M + FFixed::Unit;
			else R = M - FFixed::Unit;
			M = (L + R) >> 1;
		}
		Temp = L;
	}

	return Temp;
}
