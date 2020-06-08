// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"

class FIXEDPOINTMATH_API FFixedMath
{
public:

	// ����ת��

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

	// ������ѧ

	template<size_t DP>
	static FORCEINLINE TFixed<DP> Min(TFixed<DP> A, TFixed<DP> B) { return FMath::Min(A, B); }

	template<size_t DP>
	static FORCEINLINE TFixed<DP> Max(TFixed<DP> A, TFixed<DP> B) { return FMath::Max(A, B); }

	template<size_t DP>
	static FORCEINLINE TFixed<DP> Clamp(TFixed<DP> Value, TFixed<DP> Min, TFixed<DP> Max) { return FMath::Clamp(Value, Min, Max); }

	// ���Ǻ��� - ע�⣡0~1 ��ʾ 0~360 �ȣ�

	static TFixed<4096> Tan(TFixed<4096> A);

	static TFixed<4096> Sin(TFixed<4096> A);

	static TFixed<4096> Cos(TFixed<4096> A);
};
