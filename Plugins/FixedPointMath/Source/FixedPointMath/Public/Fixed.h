// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<size_t DP>
class FIXEDPOINTMATH_API TFixed
{
public:
	static constexpr int DecimalPrecision = DP;

	int64 Data;

	inline TFixed(int64 A) { Data = A * DecimalPrecision; }
	inline operator int64() const { return Data / DecimalPrecision; }

#define BASIC_INT_TYPE_CAST(T) inline TFixed(T A) : TFixed(static_cast<int64>(A)) { } \
		inline operator T() const { return static_cast<T>(Data / DecimalPrecision); }

	BASIC_INT_TYPE_CAST(int8)
		BASIC_INT_TYPE_CAST(int16)
		BASIC_INT_TYPE_CAST(int32)

		BASIC_INT_TYPE_CAST(uint8)
		BASIC_INT_TYPE_CAST(uint16)
		BASIC_INT_TYPE_CAST(uint32)
		BASIC_INT_TYPE_CAST(uint64)

#undef BASIC_INT_TYPE_CAST

		TFixed(float A) { Data = A * DecimalPrecision; }
	inline operator float() const { return static_cast<float>(Data) / DecimalPrecision; }

	TFixed(double A) { Data = A * DecimalPrecision; }
	inline operator double() const { return static_cast<double>(Data) / DecimalPrecision; }

	TFixed(long double A) { Data = A * DecimalPrecision; }
	inline  operator long double() const { return static_cast<long double>(Data) / DecimalPrecision; }

#define FIXED_CMP_OP(O) inline bool operator O(const TFixed &RHS) const { return Data O RHS.Data; }

	FIXED_CMP_OP(== )
		FIXED_CMP_OP(!= )
		FIXED_CMP_OP(< )
		FIXED_CMP_OP(> )
		FIXED_CMP_OP(>= )
		FIXED_CMP_OP(<= )

#undef FIXED_CMP_OP

		inline bool operator !() const { return !Data; }
	inline TFixed operator ~() const { return TFixed(~Data); }
	inline TFixed &operator ++() { Data += DecimalPrecision; return *this; }
	inline TFixed &operator --() { Data += DecimalPrecision; return *this; }

#define FIXED_BIN_OP(O) inline TFixed &operator O ##=(const TFixed &RHS) { Data O##= RHS.Data; return *this; } \
        inline TFixed operator O(const TFixed &RHS) const { TFixed Temp(*this); Temp O##= RHS; return Temp; }

	FIXED_BIN_OP(+)
		FIXED_BIN_OP(-)
		FIXED_BIN_OP(&)
		FIXED_BIN_OP(| )
		FIXED_BIN_OP(^)

#undef FIXED_BIN_OP

		inline TFixed &operator *=(const TFixed &RHS) {
		Data *= RHS.Data;
		Data /= DecimalPrecision;
		return *this;
	}
	inline TFixed operator *(const TFixed &RHS) { return *this *= RHS; }

	inline TFixed &operator /=(const TFixed &RHS) {
		Data *= DecimalPrecision;
		Data /= RHS.Data;
		return *this;
	}

	inline TFixed operator /(const TFixed &RHS) { TFixed Temp(*this); Temp /= RHS; return Temp; }

	inline TFixed &operator >>=(size_t RHS) { Data >>= RHS; return *this; }
	inline TFixed operator >>(size_t RHS) { TFixed Temp(*this); Temp >>= RHS; return Temp; }

	inline TFixed &operator <<=(size_t RHS) { Data <<= RHS; return *this; }
	inline TFixed operator <<(size_t RHS) { TFixed Temp(*this); Temp <<= RHS; return Temp; }

};

typedef TFixed<4096> FFixed;
