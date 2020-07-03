// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FORCEINLINE FFixed(int64 A, bool UseRaw = false) { Data = UseRaw ? A : A << DecimalBit; }
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

#define FIXED_CMP_OP(O) FORCEINLINE bool operator O(const FFixed& RHS) const { return Data O RHS.Data; }

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

#define FIXED_DIR_OP(O) FORCEINLINE FFixed& operator O ##=(const FFixed& RHS) { Data O##= RHS.Data; return *this; } \
        FORCEINLINE FFixed operator O(const FFixed& RHS) const { FFixed Temp(*this); Temp O##= RHS; return Temp; }

	FIXED_DIR_OP(+)
	FIXED_DIR_OP(-)
	FIXED_DIR_OP(&)
	FIXED_DIR_OP(|)
	FIXED_DIR_OP(^)

#undef FIXED_DIR_OP

	FORCEINLINE FFixed& operator *=(const FFixed& RHS) {
		Data *= RHS.Data;
		Data >>= DecimalBit;
		return *this;
	}
	FORCEINLINE FFixed operator *(const FFixed& RHS) const { FFixed Temp(*this); Temp *= RHS; return Temp; }

	FORCEINLINE FFixed& operator /=(const FFixed& RHS) {
		Data <<= DecimalBit;
		Data /= RHS.Data;
		return *this;
	}

	FORCEINLINE FFixed operator /(const FFixed& RHS) const { FFixed Temp(*this); Temp /= RHS; return Temp; }

	FORCEINLINE FFixed& operator >>=(uint64 RHS) { Data >>= RHS; return *this; }
	FORCEINLINE FFixed operator >>(uint64 RHS) const { FFixed Temp(*this); Temp >>= RHS; return Temp; }

	FORCEINLINE FFixed& operator <<=(uint64 RHS) { Data <<= RHS; return *this; }
	FORCEINLINE FFixed operator <<(uint64 RHS) const { FFixed Temp(*this); Temp <<= RHS; return Temp; }

};
