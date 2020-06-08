// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"

template<size_t DP>
struct FIXEDPOINTMATH_API TFixedVector
{
public:

	TFixed<DP> X;

	TFixed<DP> Y;

	TFixed<DP> Z;

public:

	static const TFixedVector ZeroVector;

	static const TFixedVector OneVector;

	static const TFixedVector UpVector;

	static const TFixedVector DownVector;

	static const TFixedVector ForwardVector;

	static const TFixedVector BackwardVector;

	static const TFixedVector RightVector;

	static const TFixedVector LeftVector;

public:

	FORCEINLINE TFixedVector() { }

	explicit FORCEINLINE TFixedVector(TFixed<DP> InF) : X(InF), Y(InF), Z(InF) { }

	FORCEINLINE TFixedVector(TFixed<DP> InX, TFixed<DP> InY, TFixed<DP> InZ) : X(InX), Y(InY), Z(InZ) { }

	explicit FORCEINLINE TFixedVector(FVector InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }

	explicit FORCEINLINE TFixedVector(FIntVector InVector) : X(InVector.X), Y(InVector.Y), Z(InVector.Z) { }

};

typedef TFixedVector<FFixed::DecimalPrecision> FFixedVector;
