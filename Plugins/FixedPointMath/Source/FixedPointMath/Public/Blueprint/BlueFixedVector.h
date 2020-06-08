// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueFixed.h"
#include "FixedVector.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueFixedVector.generated.h"

// 用反射结构体包裹
USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedPointVector
{
	GENERATED_BODY()

	FFixedVector Raw;

	FORCEINLINE FFixedPointVector() { }
	FORCEINLINE FFixedPointVector(FFixedVector InFixed) : Raw(InFixed) { }
};

UCLASS()
class FIXEDPOINTMATH_API UBlueprintFixedVectorMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Make/Break函数

	UFUNCTION(BlueprintPure, meta = (Keywords = "construct build", NativeMakeFunc), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector MakeFixedPointVector(FFixedPoint X, FFixedPoint Y, FFixedPoint Z) { return FFixedPointVector(FFixedVector(X.Raw, Y.Raw, Z.Raw)); }

	UFUNCTION(BlueprintPure, meta = (NativeBreakFunc), Category = "Math|FixedVector")
	static void BreakFixedPointVector(const FFixedPointVector& InVector, FFixedPoint& X, FFixedPoint& Y, FFixedPoint& Z) { X.Raw = InVector.Raw.X; Y.Raw = InVector.Raw.Y; Z.Raw = InVector.Raw.Z; }

	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedPointVector FixedToFixedVector(FFixedPoint InFixed) { return static_cast<FFixedVector>(InFixed.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (Vector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedPointVector VectorToFixedVector(FVector InVector) { return static_cast<FFixedVector>(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (IntVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedPointVector IntVectorToFixedVector(FIntVector InVector) { return static_cast<FFixedVector>(InVector); }
};
