// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// ���Ͷ�����
template<int64 DP>
class FIXEDPOINTMATH_API TFixed
{
public:
	static constexpr int64 DecimalPrecision = DP;
	
	static const TFixed Zero;
	static const TFixed One;
	static const TFixed Unit;

	int64 Data;

	FORCEINLINE TFixed() { };
	explicit FORCEINLINE TFixed(int64 A, bool UseRaw = false) { Data = UseRaw ? A : A * DecimalPrecision; }
	explicit FORCEINLINE operator int64() const { return Data / DecimalPrecision; }

	FORCEINLINE TFixed(int32 A) { Data = A * DecimalPrecision; }
	explicit FORCEINLINE operator int32() const { return Data / DecimalPrecision; }

#define BASIC_INT_TYPE_CAST(T) explicit FORCEINLINE TFixed(T A) : TFixed(static_cast<int64>(A)) { } \
		explicit FORCEINLINE operator T() const { return static_cast<T>(Data / DecimalPrecision); }

	BASIC_INT_TYPE_CAST(int8)
	BASIC_INT_TYPE_CAST(int16)

	BASIC_INT_TYPE_CAST(uint8)
	BASIC_INT_TYPE_CAST(uint16)
	BASIC_INT_TYPE_CAST(uint32)
	BASIC_INT_TYPE_CAST(uint64)

#undef BASIC_INT_TYPE_CAST

	explicit TFixed(float A) { Data = A * DecimalPrecision + 0.5; }
	explicit FORCEINLINE operator float() const { return static_cast<float>(Data) / DecimalPrecision; }

	explicit TFixed(double A) { Data = A * DecimalPrecision + 0.5; }
	FORCEINLINE operator double() const { return static_cast<double>(Data) / DecimalPrecision; }

	explicit TFixed(long double A) { Data = A * DecimalPrecision + 0.5; }
	explicit FORCEINLINE operator long double() const { return static_cast<long double>(Data) / DecimalPrecision; }

#define FIXED_CMP_OP(O) FORCEINLINE bool operator O(const TFixed &RHS) const { return Data O RHS.Data; }

	FIXED_CMP_OP(==)
	FIXED_CMP_OP(!=)
	FIXED_CMP_OP(<)
	FIXED_CMP_OP(>)
	FIXED_CMP_OP(>=)
	FIXED_CMP_OP(<=)

#undef FIXED_CMP_OP

	FORCEINLINE bool operator !() const { return !Data; }
	FORCEINLINE TFixed operator ~() const { return TFixed(~Data); }
	FORCEINLINE TFixed &operator ++() { Data += DecimalPrecision; return *this; }
	FORCEINLINE TFixed &operator --() { Data += DecimalPrecision; return *this; }
	FORCEINLINE TFixed operator -() const { TFixed Temp; Temp.Data = -Data; return Temp; }

#define FIXED_DIR_OP(O) FORCEINLINE TFixed &operator O ##=(const TFixed &RHS) { Data O##= RHS.Data; return *this; } \
        FORCEINLINE TFixed operator O(const TFixed &RHS) const { TFixed Temp(*this); Temp O##= RHS; return Temp; }

	FIXED_DIR_OP(+)
	FIXED_DIR_OP(-)
	FIXED_DIR_OP(&)
	FIXED_DIR_OP(|)
	FIXED_DIR_OP(^)

#undef FIXED_DIR_OP

	FORCEINLINE TFixed &operator *=(const TFixed &RHS) {
		Data *= RHS.Data;
		Data /= DecimalPrecision;
		return *this;
	}
	FORCEINLINE TFixed operator *(const TFixed &RHS) const { TFixed Temp(*this); Temp *= RHS; return Temp; }

	FORCEINLINE TFixed &operator /=(const TFixed &RHS) {
		Data *= DecimalPrecision;
		Data /= RHS.Data;
		return *this;
	}

	FORCEINLINE TFixed operator /(const TFixed &RHS) const { TFixed Temp(*this); Temp /= RHS; return Temp; }

	FORCEINLINE TFixed &operator >>=(uint64 RHS) { Data >>= RHS; return *this; }
	FORCEINLINE TFixed operator >>(uint64 RHS) const { TFixed Temp(*this); Temp >>= RHS; return Temp; }

	FORCEINLINE TFixed &operator <<=(uint64 RHS) { Data <<= RHS; return *this; }
	FORCEINLINE TFixed operator <<(uint64 RHS) const { TFixed Temp(*this); Temp <<= RHS; return Temp; }

};

// ʵ����һ������Ϊ4096�Ķ�����
typedef TFixed<4096> FFixed;

template<int64 DP>
const TFixed<DP> TFixed<DP>::Zero = TFixed<DP>(0);

template<int64 DP>
const TFixed<DP> TFixed<DP>::One = TFixed<DP>(1);

template<int64 DP>
const TFixed<DP> TFixed<DP>::Unit = TFixed<DP>(1, true);
