// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fixed.h"
#include "FixedMath.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueFixed.generated.h"

// �÷���ṹ�����
USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedPoint 
{
	GENERATED_BODY()
		
	FFixed Raw;

	inline FFixedPoint() { }
	inline FFixedPoint(FFixed InFixed) : Raw(InFixed) { }

	inline operator FFixed() const { return Raw; }

};

UCLASS()
class FIXEDPOINTMATH_API UBlueprintFixedMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// ����ת��

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (byte)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FFixedPoint ByteToFixed(uint8 InByte) { return InByte; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (integer)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FFixedPoint IntToFixed(int32 InInt) { return InInt; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (float)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FFixedPoint FloatToFixed(float InFloat) { return InFloat; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToByte (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static uint8 FixedToByte(FFixedPoint InFixed) { return static_cast<FFixed>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToInt (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static int32 FixedToInt(FFixedPoint InFixed) { return static_cast<FFixed>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFloat (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static float FixedToFloat(FFixedPoint InFixed) { return static_cast<FFixed>(InFixed); }

	// ��������

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed * fixed", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FFixedPoint Multiply(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) * B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed / fixed", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|Fixed")
	static FFixedPoint Divide(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) / (static_cast<FFixed>(B) == static_cast<FFixed>(0) ? static_cast<FFixed>(1) : B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed + fixed", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FFixedPoint Add(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed - fixed", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|Fixed")
	static FFixedPoint Subtract(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) - B; }

	// ��������

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed < fixed", CompactNodeTitle = "<", Keywords = "< less"), Category = "Math|Fixed")
	static bool Less(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed > fixed", CompactNodeTitle = ">", Keywords = "> greater"), Category = "Math|Fixed")
	static bool Greater(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed <= fixed", CompactNodeTitle = "<=", Keywords = "<= less"), Category = "Math|Fixed")
	static bool LessEqual(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) <= B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed >= fixed", CompactNodeTitle = ">=", Keywords = ">= greater"), Category = "Math|Fixed")
	static bool GreaterEqual(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) >= B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (fixed)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|Fixed")
	static bool EqualEqual(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (fixed)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|Fixed")
	static bool NotEqual(FFixedPoint A, FFixedPoint B) { return static_cast<FFixed>(A) != B; }

	// ������ѧ

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (fixed)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FFixedPoint Min(FFixedPoint A, FFixedPoint B) { return FFixedMath::Min(static_cast<FFixed>(A), static_cast<FFixed>(B)); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (fixed)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FFixedPoint Max(FFixedPoint A, FFixedPoint B) { return FFixedMath::Max(static_cast<FFixed>(A), static_cast<FFixed>(B)); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Clamp (fixed)"), Category = "Math|Fixed")
	static FFixedPoint Clamp(FFixedPoint Value, FFixedPoint Min, FFixedPoint Max) { return FFixedMath::Clamp(static_cast<FFixed>(Value), static_cast<FFixed>(Min), static_cast<FFixed>(Max)); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "InRange (fixed)", Min = "0", Max = "10"), Category = "Math|Fixed")
	static bool InRange(FFixedPoint Value, FFixedPoint Min, FFixedPoint Max, bool InclusiveMin = true, bool InclusiveMax = true) 
	{ return ((InclusiveMin ? (static_cast<FFixed>(Value) >= Min) : (static_cast<FFixed>(Value) > Min)) && (InclusiveMax ? (static_cast<FFixed>(Value) <= Max) : (static_cast<FFixed>(Value) < Max))); }

	// ���Ǻ��� - ע�⣡0~1 ��ʾ 0~360 �ȣ�

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Fixed)", CompactNodeTitle = "TAN"), Category = "Math|Trig")
	static FFixedPoint Tan(FFixedPoint A) { return FFixedMath::PrecConv<FFixed::DecimalPrecision>(FFixedMath::Tan(FFixedMath::PrecConv<4096>(static_cast<FFixed>(A)))); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Fixed)", CompactNodeTitle = "SIN"), Category = "Math|Trig")
	static FFixedPoint Sin(FFixedPoint A) { return FFixedMath::PrecConv<FFixed::DecimalPrecision>(FFixedMath::Sin(FFixedMath::PrecConv<4096>(static_cast<FFixed>(A)))); }
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Fixed)", CompactNodeTitle = "COS"), Category = "Math|Trig")
	static FFixedPoint Cos(FFixedPoint A) { return FFixedMath::PrecConv<FFixed::DecimalPrecision>(FFixedMath::Cos(FFixedMath::PrecConv<4096>(static_cast<FFixed>(A)))); }
};
