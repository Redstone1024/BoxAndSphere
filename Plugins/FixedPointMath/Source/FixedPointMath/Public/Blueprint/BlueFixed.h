// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fixed.h"
#include "FixedMath.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueFixed.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "FixedMathLibrary"))
class FIXEDPOINTMATH_API UBlueprintFixedMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (byte)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixed ByteToFixed(uint8 InByte) { return static_cast<FFixed>(InByte); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (integer)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixed IntToFixed(int32 InInt) { return static_cast<FFixed>(InInt); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (float)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixed FloatToFixed(float InFloat) { return static_cast<FFixed>(InFloat); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToByte (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE uint8 FixedToByte(FFixed InFixed) { return static_cast<uint8>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToInt (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE int32 FixedToInt(FFixed InFixed) { return static_cast<int32>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFloat (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE float FixedToFloat(FFixed InFixed) { return static_cast<float>(InFixed); }

	// 四则运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed * fixed", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Multiply(FFixed A, FFixed B) { return A * B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed / fixed", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Divide(FFixed A, FFixed B) { return A / (B == 0 ? 1 : B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed + fixed", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Add(FFixed A, FFixed B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed - fixed", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Subtract(FFixed A, FFixed B) { return A - B; }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed < fixed", CompactNodeTitle = "<", Keywords = "< less"), Category = "Math|Fixed")
	static FORCEINLINE bool Less(FFixed A, FFixed B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed > fixed", CompactNodeTitle = ">", Keywords = "> greater"), Category = "Math|Fixed")
	static FORCEINLINE bool Greater(FFixed A, FFixed B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed <= fixed", CompactNodeTitle = "<=", Keywords = "<= less"), Category = "Math|Fixed")
	static FORCEINLINE bool LessEqual(FFixed A, FFixed B) { return A <= B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed >= fixed", CompactNodeTitle = ">=", Keywords = ">= greater"), Category = "Math|Fixed")
	static FORCEINLINE bool GreaterEqual(FFixed A, FFixed B) { return A >= B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (fixed)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|Fixed")
	static FORCEINLINE bool EqualEqual(FFixed A, FFixed B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (fixed)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|Fixed")
	static FORCEINLINE bool NotEqual(FFixed A, FFixed B) { return A != A; }

	// 常用数学

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Absolute (fixed)", CompactNodeTitle = "ABS"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Abs(FFixed A) { return FFixedMath::Abs(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (fixed)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Min(FFixed A, FFixed B) { return FFixedMath::Min(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (fixed)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Max(FFixed A, FFixed B) { return FFixedMath::Max(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Clamp (fixed)"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Clamp(FFixed Value, FFixed Min, FFixed Max) { return FFixedMath::Clamp(Value, Min, Max); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "InRange (fixed)", Min = "0", Max = "10"), Category = "Math|Fixed")
	static FORCEINLINE bool InRange(FFixed Value, FFixed Min, FFixed Max, bool InclusiveMin = true, bool InclusiveMax = true) 
	{ return ((InclusiveMin ? (Value >= Min) : (Value > Min)) && (InclusiveMax ? (Value <= Max) : (Value < Max))); }

	UFUNCTION(BlueprintPure, Category = "Math|Fixed", meta = (Keywords = "square root", CompactNodeTitle = "SQRT"))
	static FORCEINLINE FFixed Sqrt(FFixed A)  { return FFixedMath::Sqrt(A); }

	// 三角函数

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get PI (fixed)", CompactNodeTitle = "PI"), Category = "Math|Trig")
	static FORCEINLINE FFixed GetPI() { return FFixed::Pi; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Radians)", CompactNodeTitle = "TAN"), Category = "Math|Trig")
	static FORCEINLINE FFixed Tan(FFixed A) { return FFixedMath::Tan(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Radians)", CompactNodeTitle = "SIN"), Category = "Math|Trig")
	static FORCEINLINE FFixed Sin(FFixed A) { return FFixedMath::Sin(A); }
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Radians)", CompactNodeTitle = "COS"), Category = "Math|Trig")
	static FORCEINLINE FFixed Cos(FFixed A) { return FFixedMath::Cos(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Degrees)", CompactNodeTitle = "TANd"), Category = "Math|Trig")
	static FORCEINLINE FFixed TanDeg(FFixed A) { return FFixedMath::TanDeg(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Degrees)", CompactNodeTitle = "SINd"), Category = "Math|Trig")
	static FORCEINLINE FFixed SinDeg(FFixed A) { return FFixedMath::SinDeg(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Degrees)", CompactNodeTitle = "COSd"), Category = "Math|Trig")
	static FORCEINLINE FFixed CosDeg(FFixed A) { return FFixedMath::CosDeg(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Unit)", CompactNodeTitle = "TANu"), Category = "Math|Trig")
	static FORCEINLINE FFixed TanUnit(FFixed A) { return FFixedMath::TanUnit(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Unit)", CompactNodeTitle = "SINu"), Category = "Math|Trig")
	static FORCEINLINE FFixed SinUnit(FFixed A) { return FFixedMath::SinUnit(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Unit)", CompactNodeTitle = "COSu"), Category = "Math|Trig")
	static FORCEINLINE FFixed CosUnit(FFixed A) { return FFixedMath::CosUnit(A); }
	
};
