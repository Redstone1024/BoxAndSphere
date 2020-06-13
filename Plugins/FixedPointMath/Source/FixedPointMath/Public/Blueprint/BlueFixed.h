// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fixed.h"
#include "FixedMath.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueFixed.generated.h"

// 用反射结构体包裹
USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedPoint 
{
	GENERATED_BODY()
		
	FFixed Raw;

	FORCEINLINE FFixedPoint() { }
	FORCEINLINE FFixedPoint(FFixed InFixed) : Raw(InFixed) { }
};

UCLASS()
class FIXEDPOINTMATH_API UBlueprintFixedMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (byte)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedPoint ByteToFixed(uint8 InByte) { return static_cast<FFixed>(InByte); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (integer)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedPoint IntToFixed(int32 InInt) { return static_cast<FFixed>(InInt); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (float)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedPoint FloatToFixed(float InFloat) { return static_cast<FFixed>(InFloat); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToByte (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE uint8 FixedToByte(FFixedPoint InFixed) { return static_cast<uint8>(InFixed.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToInt (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE int32 FixedToInt(FFixedPoint InFixed) { return static_cast<int32>(InFixed.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFloat (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE float FixedToFloat(FFixedPoint InFixed) { return static_cast<float>(InFixed.Raw); }

	// 四则运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed * fixed", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Multiply(FFixedPoint A, FFixedPoint B) { return A.Raw * B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed / fixed", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Divide(FFixedPoint A, FFixedPoint B) { return A.Raw / (B.Raw == FFixed::Zero ? FFixed::One : B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed + fixed", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Add(FFixedPoint A, FFixedPoint B) { return A.Raw + B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed - fixed", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Subtract(FFixedPoint A, FFixedPoint B) { return A.Raw - B.Raw; }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed < fixed", CompactNodeTitle = "<", Keywords = "< less"), Category = "Math|Fixed")
	static FORCEINLINE bool Less(FFixedPoint A, FFixedPoint B) { return A.Raw < B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed > fixed", CompactNodeTitle = ">", Keywords = "> greater"), Category = "Math|Fixed")
	static FORCEINLINE bool Greater(FFixedPoint A, FFixedPoint B) { return A.Raw > B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed <= fixed", CompactNodeTitle = "<=", Keywords = "<= less"), Category = "Math|Fixed")
	static FORCEINLINE bool LessEqual(FFixedPoint A, FFixedPoint B) { return A.Raw <= B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed >= fixed", CompactNodeTitle = ">=", Keywords = ">= greater"), Category = "Math|Fixed")
	static FORCEINLINE bool GreaterEqual(FFixedPoint A, FFixedPoint B) { return A.Raw >= B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (fixed)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|Fixed")
	static FORCEINLINE bool EqualEqual(FFixedPoint A, FFixedPoint B) { return A.Raw == B.Raw; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (fixed)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|Fixed")
	static FORCEINLINE bool NotEqual(FFixedPoint A, FFixedPoint B) { return A.Raw != A.Raw; }

	// 常用数学

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Absolute (fixed)", CompactNodeTitle = "ABS"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Abs(FFixedPoint A) { return FFixedMath::Abs(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (fixed)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Min(FFixedPoint A, FFixedPoint B) { return FFixedMath::Min(A.Raw, B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (fixed)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Max(FFixedPoint A, FFixedPoint B) { return FFixedMath::Max(A.Raw, B.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Clamp (fixed)"), Category = "Math|Fixed")
	static FORCEINLINE FFixedPoint Clamp(FFixedPoint Value, FFixedPoint Min, FFixedPoint Max) { return FFixedMath::Clamp(Value.Raw, Min.Raw, Max.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "InRange (fixed)", Min = "0", Max = "10"), Category = "Math|Fixed")
	static FORCEINLINE bool InRange(FFixedPoint Value, FFixedPoint Min, FFixedPoint Max, bool InclusiveMin = true, bool InclusiveMax = true) 
	{ return ((InclusiveMin ? (Value.Raw >= Min.Raw) : (Value.Raw > Min.Raw)) && (InclusiveMax ? (Value.Raw <= Max.Raw) : (Value.Raw < Max.Raw))); }

	UFUNCTION(BlueprintPure, Category = "Math|Fixed", meta = (Keywords = "square root", CompactNodeTitle = "SQRT"))
	static FORCEINLINE FFixedPoint Sqrt(FFixedPoint A)  { return FFixedMath::Sqrt(A.Raw); }

	// 三角函数

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get PI (fixed)", CompactNodeTitle = "PI"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint GetPI() { return FFixedMath::GetPI<FFixed::DecimalPrecision>(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Radians)", CompactNodeTitle = "TAN"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint Tan(FFixedPoint A) { return FFixedMath::Tan(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Radians)", CompactNodeTitle = "SIN"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint Sin(FFixedPoint A) { return FFixedMath::Sin(A.Raw); }
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Radians)", CompactNodeTitle = "COS"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint Cos(FFixedPoint A) { return FFixedMath::Cos(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Degrees)", CompactNodeTitle = "TANd"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint TanDeg(FFixedPoint A) { return FFixedMath::TanDeg(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Degrees)", CompactNodeTitle = "SINd"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint SinDeg(FFixedPoint A) { return FFixedMath::SinDeg(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Degrees)", CompactNodeTitle = "COSd"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint CosDeg(FFixedPoint A) { return FFixedMath::CosDeg(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Unit)", CompactNodeTitle = "TANu"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint TanUnit(FFixedPoint A) { return FFixedMath::TanUnit(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Unit)", CompactNodeTitle = "SINu"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint SinUnit(FFixedPoint A) { return FFixedMath::SinUnit(A.Raw); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Unit)", CompactNodeTitle = "COSu"), Category = "Math|Trig")
	static FORCEINLINE FFixedPoint CosUnit(FFixedPoint A) { return FFixedMath::CosUnit(A.Raw); }

};
