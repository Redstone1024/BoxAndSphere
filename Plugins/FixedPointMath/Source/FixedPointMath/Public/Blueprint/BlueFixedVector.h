// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueFixed.h"
#include "FixedVector.h"
#include "FixedVectorMath.h"
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

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector (FixedVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FVector FixedVectorToVector(FFixedPointVector InVector) { return FVector(InVector.Raw.X, InVector.Raw.Y, InVector.Raw.Z); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToIntVector (FixedVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FIntVector FixedVectorToIntVector(FFixedPointVector InVector) { return FIntVector(InVector.Raw.X, InVector.Raw.Y, InVector.Raw.Z); }

	// 四则运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector * FixedVector", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Multiply(FFixedPointVector A, FFixedPointVector B) { return A.Raw * B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector / FixedVector", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Divide(FFixedPointVector A, FFixedPointVector B) { return A.Raw / (!(B.Raw != static_cast<FFixedVector>(0)) ? static_cast<FFixedVector>(1) : B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector + FixedVector", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Add(FFixedPointVector A, FFixedPointVector B) { return A.Raw + B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector - FixedVector", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Subtract(FFixedPointVector A, FFixedPointVector B) { return A.Raw - B.Raw; }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (FixedVector)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|FixedVector")
	static FORCEINLINE bool EqualEqual(FFixedPointVector A, FFixedPointVector B) { return A.Raw == B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (FixedVector)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|FixedVector")
	static FORCEINLINE bool NotEqual(FFixedPointVector A, FFixedPointVector B) { return A.Raw != A.Raw; }

	// 常用数学

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Absolute (FixedVector)", CompactNodeTitle = "ABS"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Abs(FFixedPointVector A) { return FFixedVectorMath::ComponentAbs(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (FixedVector)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Min(FFixedPointVector A, FFixedPointVector B) { return FFixedVectorMath::ComponentMin(A.Raw, B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (FixedVector)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Max(FFixedPointVector A, FFixedPointVector B) { return FFixedVectorMath::ComponentMax(A.Raw, B.Raw); }

	// 向量运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cross Product", CompactNodeTitle = "cross", ScriptMethod = "Cross", ScriptOperator = "^"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector CrossProduct(const FFixedPointVector& A, const FFixedPointVector& B) { return FFixedVectorMath::CrossProduct(A.Raw, B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Dot Product", CompactNodeTitle = "dot", ScriptMethod = "Dot", ScriptOperator = "|"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPoint DotProduct(const FFixedPointVector& A, const FFixedPointVector& B) { return FFixedVectorMath::DotProduct(A.Raw, B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VectorLength", ScriptMethod = "Length", Keywords = "magnitude"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPoint Length(const FFixedPointVector& A) { return A.Raw.Length(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VectorLengthSquared", ScriptMethod = "LengthSquared", Keywords = "magnitude"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPoint LengthSquared(const FFixedPointVector& A) { return A.Raw.LengthSquared(); }

	UFUNCTION(BlueprintPure, meta = (ScriptMethod = "IsZero"), Category = "Math|FixedVector")
	static FORCEINLINE bool IsZero(const FFixedPointVector& A) { return A.Raw.IsZero(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Normalize (FixedVector)", ScriptMethod = "Normalize", Keywords = "Unit Vector"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedPointVector Normalize(const FFixedPointVector& A) { return FFixedVectorMath::Normalize(A.Raw); }

};
