// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueFixed.h"
#include "FixedVector.h"
#include "FixedVectorMath.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueFixedVector.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "FixedVectorMathLibrary"))
class FIXEDPOINTMATH_API UBlueprintFixedVectorMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedVector FixedToFixedVector(FFixed InFixed) { return static_cast<FFixedVector>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (Vector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedVector VectorToFixedVector(FVector InVector) { return static_cast<FFixedVector>(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (IntVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedVector IntVectorToFixedVector(FIntVector InVector) { return static_cast<FFixedVector>(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector (FixedVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FVector FixedVectorToVector(FFixedVector InVector) { return FVector(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToIntVector (FixedVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FIntVector FixedVectorToIntVector(FFixedVector InVector) { return FIntVector(InVector); }

	// 四则运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector * FixedVector", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Multiply(FFixedVector A, FFixedVector B) { return A * B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector / FixedVector", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Divide(FFixedVector A, FFixedVector B) { return A / (!(B != static_cast<FFixedVector>(0)) ? static_cast<FFixedVector>(1) : B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector + FixedVector", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Add(FFixedVector A, FFixedVector B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector - FixedVector", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Subtract(FFixedVector A, FFixedVector B) { return A - B; }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (FixedVector)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|FixedVector")
	static FORCEINLINE bool EqualEqual(FFixedVector A, FFixedVector B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (FixedVector)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|FixedVector")
	static FORCEINLINE bool NotEqual(FFixedVector A, FFixedVector B) { return A != A; }

	// 常用数学

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Absolute (FixedVector)", CompactNodeTitle = "ABS"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Abs(FFixedVector A) { return FFixedVectorMath::ComponentAbs(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (FixedVector)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Min(FFixedVector A, FFixedVector B) { return FFixedVectorMath::ComponentMin(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (FixedVector)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Max(FFixedVector A, FFixedVector B) { return FFixedVectorMath::ComponentMax(A, B); }

	// 向量运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cross Product", CompactNodeTitle = "cross", ScriptMethod = "Cross", ScriptOperator = "^"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector CrossProduct(const FFixedVector& A, const FFixedVector& B) { return FFixedVectorMath::CrossProduct(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Dot Product", CompactNodeTitle = "dot", ScriptMethod = "Dot", ScriptOperator = "|"), Category = "Math|FixedVector")
	static FORCEINLINE FFixed DotProduct(const FFixedVector& A, const FFixedVector& B) { return FFixedVectorMath::DotProduct(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VectorLength", ScriptMethod = "Length", Keywords = "magnitude"), Category = "Math|FixedVector")
	static FORCEINLINE FFixed Length(const FFixedVector& A) { return A.Length(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VectorLengthSquared", ScriptMethod = "LengthSquared", Keywords = "magnitude"), Category = "Math|FixedVector")
	static FORCEINLINE FFixed LengthSquared(const FFixedVector& A) { return A.LengthSquared(); }

	UFUNCTION(BlueprintPure, meta = (ScriptMethod = "IsZero"), Category = "Math|FixedVector")
	static FORCEINLINE bool IsZero(const FFixedVector& A) { return A.IsZero(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Normalize (FixedVector)", ScriptMethod = "Normalize", Keywords = "Unit Vector"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Normalize(const FFixedVector& A) { return FFixedVectorMath::Normalize(A); }

};
