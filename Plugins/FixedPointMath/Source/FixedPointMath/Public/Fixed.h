// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FixedPointMath.h"
#include "Fixed.generated.h"

USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed", SaveGame)
	int64 Data;

	static const FFixed Unit;
	static const FFixed Pi;

	static constexpr unsigned int DecimalBit = 12;

	FORCEINLINE FFixed() { };
	FORCEINLINE FFixed(int64 A) { Data = A << DecimalBit; }
	explicit FORCEINLINE operator int64() const { return Data >> DecimalBit; }

	FORCEINLINE FFixed(int32 A) : FFixed(static_cast<int64>(A)) { }
	explicit FORCEINLINE operator int32() const { return static_cast<int32>(Data >> DecimalBit); }

#define BASIC_INT_TYPE_CAST(T) explicit FORCEINLINE FFixed(T A) : FFixed(static_cast<int64>(A)) { } \
		explicit FORCEINLINE operator T() const { return static_cast<T>(Data >> DecimalBit); }

	BASIC_INT_TYPE_CAST(int8)
	BASIC_INT_TYPE_CAST(int16)

	BASIC_INT_TYPE_CAST(uint8)
	BASIC_INT_TYPE_CAST(uint16)
	BASIC_INT_TYPE_CAST(uint32)
	BASIC_INT_TYPE_CAST(uint64)

#undef BASIC_INT_TYPE_CAST

	explicit FFixed(float A) { Data = A * (1 << DecimalBit) + 0.5; }
	explicit FORCEINLINE operator float() const { return static_cast<float>(Data) / (1 << DecimalBit); }

	explicit FFixed(double A) { Data = A * (1 << DecimalBit) + 0.5; }
	explicit FORCEINLINE operator double() const { return static_cast<double>(Data) / (1 << DecimalBit); }

	explicit FFixed(long double A) { Data = A * (1 << DecimalBit) + 0.5; }
	explicit FORCEINLINE operator long double() const { return static_cast<long double>(Data) / (1 << DecimalBit); }

	static FFixed FromBit(int64 A) { FFixed Temp; Temp.Data = A; return Temp; }

#define FIXED_CMP_OP(O) friend FORCEINLINE bool operator O(const FFixed A, const FFixed B) { return A.Data O B.Data; }

	FIXED_CMP_OP(==)
	FIXED_CMP_OP(!=)
	FIXED_CMP_OP(<)
	FIXED_CMP_OP(>)
	FIXED_CMP_OP(>=)
	FIXED_CMP_OP(<=)

#undef FIXED_CMP_OP

	FORCEINLINE bool operator !() const { return !Data; }
	FORCEINLINE FFixed operator ~() const { return FFixed(~Data); }
	FORCEINLINE FFixed& operator ++() { Data += (1 << DecimalBit); return *this; }
	FORCEINLINE FFixed& operator --() { Data += (1 << DecimalBit); return *this; }
	FORCEINLINE FFixed operator -() const { FFixed Temp; Temp.Data = -Data; return Temp; }

#define FIXED_NAS_OP(O) friend FORCEINLINE FFixed operator O(FFixed A, FFixed B) { A O##= B; return A; }
#define FIXED_DIR_OP(O) FORCEINLINE FFixed& operator O ##=(const FFixed F) { Data O##= F.Data; return *this; } FIXED_NAS_OP(O)

	FIXED_DIR_OP(+)
	FIXED_DIR_OP(-)
	FIXED_DIR_OP(&)
	FIXED_DIR_OP(|)
	FIXED_DIR_OP(^)

#undef FIXED_DIR_OP

	FORCEINLINE FFixed& operator *=(const FFixed& F) {
		Data *= F.Data;
		Data >>= DecimalBit;
		return *this;
	}

	FORCEINLINE FFixed& operator /=(const FFixed& F) {
		Data <<= DecimalBit;
		Data /= F.Data;
		return *this;
	}

	FORCEINLINE FFixed& operator %=(const FFixed& F) {
		Data %= F.Data;
		return *this;
	}

	FIXED_NAS_OP(*)
	FIXED_NAS_OP(/)
	FIXED_NAS_OP(%)

#undef FIXED_NAS_OP

	FORCEINLINE FFixed& operator >>=(uint64 F) { Data >>= F; return *this; }
	FORCEINLINE FFixed operator >>(uint64 F) const { FFixed Temp(*this); Temp >>= F; return Temp; }
	FORCEINLINE FFixed& operator <<=(uint64 F) { Data <<= F; return *this; }
	FORCEINLINE FFixed operator <<(uint64 F) const { FFixed Temp(*this); Temp <<= F; return Temp; }
};

namespace FFixedMath
{
	// ������ѧ

	FORCEINLINE FFixed Abs(FFixed A) { return FMath::Abs(A); }
	FORCEINLINE FFixed Min(FFixed A, FFixed B) { return FMath::Min(A, B); }
	FORCEINLINE FFixed Max(FFixed A, FFixed B) { return FMath::Max(A, B); }
	FORCEINLINE FFixed Lerp(FFixed A, FFixed B, FFixed Alpha) { return FMath::Lerp(A, B, Alpha); }
	FORCEINLINE FFixed Clamp(FFixed Value, FFixed Min, FFixed Max) { return FMath::Clamp(Value, Min, Max); }
	FORCEINLINE FFixed Sqrt(FFixed A);

	// ���Ǻ���

	FIXEDPOINTMATH_API FFixed TanUnit(FFixed A);
	FIXEDPOINTMATH_API FFixed SinUnit(FFixed A);
	FIXEDPOINTMATH_API FFixed CosUnit(FFixed A);
	FORCEINLINE FFixed TanDeg(FFixed A) { return TanUnit(A / FFixed(360)); }
	FORCEINLINE FFixed SinDeg(FFixed A) { return SinUnit(A / FFixed(360)); }
	FORCEINLINE FFixed CosDeg(FFixed A) { return CosUnit(A / FFixed(360)); }
	FORCEINLINE FFixed Tan(FFixed A) { return TanUnit(A / (FFixed(2) * FFixed::Pi)); }
	FORCEINLINE FFixed Sin(FFixed A) { return SinUnit(A / (FFixed(2) * FFixed::Pi)); }
	FORCEINLINE FFixed Cos(FFixed A) { return CosUnit(A / (FFixed(2) * FFixed::Pi)); }
}

FORCEINLINE FFixed FFixedMath::Sqrt(FFixed A)
{
	if (A < 0)
	{
		UE_LOG(LogFixedPointMath, Warning, TEXT("The square root cannot be negative. (An overflow may have occurred!)"));
		return 0;
	}

	// TODO: ���˴��Ķ�����⻻�ɸ�����㷨

	static const FFixed DefaultL = 0;
	static const FFixed DefaultR = (int64)UINT32_MAX >> FFixed::DecimalBit;

	FFixed Temp = 0;
	FFixed L = DefaultL;
	FFixed R = DefaultR;
	FFixed M = L + ((R - L) >> 1);
	while (L < R) {
		if (M * M <= A) L = M + FFixed::Unit, Temp = M;
		else R = M - FFixed::Unit;
		M = L + ((R - L) >> 1);
	}

	return Temp;
}
