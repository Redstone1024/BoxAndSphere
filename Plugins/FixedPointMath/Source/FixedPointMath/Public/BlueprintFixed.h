// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fixed.h"
#include "FixedVector.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FixedRotator.h"
#include "BlueprintFixed.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "FixedMathLibrary"))
class FIXEDPOINTMATH_API UBlueprintFixedMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 一般：
	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (byte)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixed Conv_ByteToFixed(uint8 InByte) { return static_cast<FFixed>(InByte); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (integer)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixed Conv_IntToFixed(int32 InInt) { return static_cast<FFixed>(InInt); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixed (float)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixed Conv_FloatToFixed(float InFloat) { return static_cast<FFixed>(InFloat); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToByte (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE uint8 Conv_FixedToByte(FFixed InFixed) { return static_cast<uint8>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToInt (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE int32 Conv_FixedToInt(FFixed InFixed) { return static_cast<int32>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFloat (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE float Conv_FixedToFloat(FFixed InFixed) { return static_cast<float>(InFixed); }

	// 四则运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed * fixed", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Multiply_FixedFixed(FFixed A, FFixed B) { return A * B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed / fixed", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|Fixed")
	static FFixed Divide_FixedFixed(FFixed A, FFixed B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed + fixed", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Add_FixedFixed(FFixed A, FFixed B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed - fixed", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Subtract_FixedFixed(FFixed A, FFixed B) { return A - B; }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed < fixed", CompactNodeTitle = "<", Keywords = "< less"), Category = "Math|Fixed")
	static FORCEINLINE bool Less_FixedFixed(FFixed A, FFixed B) { return A < B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed > fixed", CompactNodeTitle = ">", Keywords = "> greater"), Category = "Math|Fixed")
	static FORCEINLINE bool Greater_FixedFixed(FFixed A, FFixed B) { return A > B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed <= fixed", CompactNodeTitle = "<=", Keywords = "<= less"), Category = "Math|Fixed")
	static FORCEINLINE bool LessEqual_FixedFixed(FFixed A, FFixed B) { return A <= B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "fixed >= fixed", CompactNodeTitle = ">=", Keywords = ">= greater"), Category = "Math|Fixed")
	static FORCEINLINE bool GreaterEqual_FixedFixed(FFixed A, FFixed B) { return A >= B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (fixed)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|Fixed")
	static FORCEINLINE bool EqualEqual_FixedFixed(FFixed A, FFixed B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (fixed)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|Fixed")
	static FORCEINLINE bool NotEqual_FixedFixed(FFixed A, FFixed B) { return A != A; }

	// 常用数学

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Absolute (fixed)", CompactNodeTitle = "ABS"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Abs_Fixed(FFixed A) { return FFixedMath::Abs(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (fixed)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Min_FixedFixed(FFixed A, FFixed B) { return FFixedMath::Min(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (fixed)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Max_FixedFixed(FFixed A, FFixed B) { return FFixedMath::Max(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Clamp (fixed)"), Category = "Math|Fixed")
	static FORCEINLINE FFixed Clamp_Fixed(FFixed Value, FFixed Min, FFixed Max) { return FFixedMath::Clamp(Value, Min, Max); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "InRange (fixed)", Min = "0", Max = "10"), Category = "Math|Fixed")
	static FORCEINLINE bool InRange_Fixed(FFixed Value, FFixed Min, FFixed Max, bool InclusiveMin = true, bool InclusiveMax = true)
	{ return ((InclusiveMin ? (Value >= Min) : (Value > Min)) && (InclusiveMax ? (Value <= Max) : (Value < Max))); }

	UFUNCTION(BlueprintPure, Category = "Math|Fixed", meta = (Keywords = "square root", CompactNodeTitle = "SQRT"))
	static FFixed Sqrt_Fixed(FFixed A);

	// 三角函数

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get PI (fixed)", CompactNodeTitle = "PI"), Category = "Math|Trig")
	static FORCEINLINE FFixed GetPI_Fixed() { return FFixed::Pi; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Radians)", CompactNodeTitle = "TAN"), Category = "Math|Trig")
	static FORCEINLINE FFixed Tan_Fixed(FFixed A) { return FFixedMath::Tan(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Radians)", CompactNodeTitle = "SIN"), Category = "Math|Trig")
	static FORCEINLINE FFixed Sin_Fixed(FFixed A) { return FFixedMath::Sin(A); }
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Radians)", CompactNodeTitle = "COS"), Category = "Math|Trig")
	static FORCEINLINE FFixed Cos_Fixed(FFixed A) { return FFixedMath::Cos(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Degrees)", CompactNodeTitle = "TANd"), Category = "Math|Trig")
	static FORCEINLINE FFixed TanDeg_Fixed(FFixed A) { return FFixedMath::TanDeg(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Degrees)", CompactNodeTitle = "SINd"), Category = "Math|Trig")
	static FORCEINLINE FFixed SinDeg_Fixed(FFixed A) { return FFixedMath::SinDeg(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Degrees)", CompactNodeTitle = "COSd"), Category = "Math|Trig")
	static FORCEINLINE FFixed CosDeg_Fixed(FFixed A) { return FFixedMath::CosDeg(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tan (Unit)", CompactNodeTitle = "TANu"), Category = "Math|Trig")
	static FORCEINLINE FFixed TanUnit_Fixed(FFixed A) { return FFixedMath::TanUnit(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sin (Unit)", CompactNodeTitle = "SINu"), Category = "Math|Trig")
	static FORCEINLINE FFixed SinUnit_Fixed(FFixed A) { return FFixedMath::SinUnit(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cos (Unit)", CompactNodeTitle = "COSu"), Category = "Math|Trig")
	static FORCEINLINE FFixed CosUnit_Fixed(FFixed A) { return FFixedMath::CosUnit(A); }

	// 向量：
	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (fixed)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedVector Conv_FixedToFixedVector(FFixed InFixed) { return static_cast<FFixedVector>(InFixed); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (Vector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedVector Conv_VectorToFixedVector(FVector InVector) { return static_cast<FFixedVector>(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedVector (IntVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedVector Conv_IntVectorToFixedVector(FIntVector InVector) { return static_cast<FFixedVector>(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToVector (FixedVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FVector Conv_FixedVectorToVector(FFixedVector InVector) { return FVector(InVector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToIntVector (FixedVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FIntVector Conv_FixedVectorToIntVector(FFixedVector InVector) { return FIntVector(InVector); }

	// 四则运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector * FixedVector", CompactNodeTitle = "*", Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Multiply_FixedVectorFixedVector(FFixedVector A, FFixedVector B) { return A * B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector / FixedVector", CompactNodeTitle = "/", Keywords = "/ divide division"), Category = "Math|FixedVector")
	static FFixedVector Divide_FixedVectorFixedVector(FFixedVector A, FFixedVector B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector + FixedVector", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Add_FixedVectorFixedVector(FFixedVector A, FFixedVector B) { return A + B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FixedVector - FixedVector", CompactNodeTitle = "-", Keywords = "- subtract minus"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Subtract_FixedVectorFixedVector(FFixedVector A, FFixedVector B) { return A - B; }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (FixedVector)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math|FixedVector")
	static FORCEINLINE bool EqualEqual_FixedVectorFixedVector(FFixedVector A, FFixedVector B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqual (FixedVector)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Math|FixedVector")
	static FORCEINLINE bool NotEqual_FixedVectorFixedVector(FFixedVector A, FFixedVector B) { return A != A; }

	// 常用数学

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Absolute (FixedVector)", CompactNodeTitle = "ABS"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Abs_FixedVector(FFixedVector A) { return FFixedMath::ComponentAbs(A); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min (FixedVector)", CompactNodeTitle = "MIN", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Min_FixedVector(FFixedVector A, FFixedVector B) { return FFixedMath::ComponentMin(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Max (FixedVector)", CompactNodeTitle = "MAX", CommutativeAssociativeBinaryOperator = "true"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Max_FixedVector(FFixedVector A, FFixedVector B) { return FFixedMath::ComponentMax(A, B); }

	UFUNCTION(BlueprintPure, meta = (ScriptMethod = "Negated", ScriptOperator = "neg"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector NegateFixedVector(FFixedVector A) { return -A; }

	// 向量运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Cross Product", CompactNodeTitle = "cross", ScriptMethod = "Cross", ScriptOperator = "^"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Cross_FixedVectorFixedVector(const FFixedVector& A, const FFixedVector& B) { return FFixedMath::CrossProduct(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Dot Product", CompactNodeTitle = "dot", ScriptMethod = "Dot", ScriptOperator = "|"), Category = "Math|FixedVector")
	static FORCEINLINE FFixed Dot_FixedVectorFixedVector(const FFixedVector& A, const FFixedVector& B) { return FFixedMath::DotProduct(A, B); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VectorLength", ScriptMethod = "Length", Keywords = "magnitude"), Category = "Math|FixedVector")
	static FORCEINLINE FFixed Length_FixedVector(const FFixedVector& A) { return A.Length(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "VectorLengthSquared", ScriptMethod = "LengthSquared", Keywords = "magnitude"), Category = "Math|FixedVector")
	static FORCEINLINE FFixed LengthSquared_FixedVector(const FFixedVector& A) { return A.LengthSquared(); }

	UFUNCTION(BlueprintPure, meta = (ScriptMethod = "IsZero"), Category = "Math|FixedVector")
	static FORCEINLINE bool IsZero_FixedVector(const FFixedVector& A) { return A.IsZero(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Normalize (FixedVector)", ScriptMethod = "Normalize", Keywords = "Unit Vector"), Category = "Math|FixedVector")
	static FORCEINLINE FFixedVector Normalize_FixedVector(const FFixedVector& A) { return FFixedMath::Normalize(A); }

	// 旋转：
	// Make/Break

	UFUNCTION(BlueprintPure, Category = "Math|FixedRotator", meta = (Keywords = "construct build rotation rotate rotator makerotator", NativeMakeFunc))
	static FFixedRotator MakeFixedRotator(UPARAM(DisplayName = "X (Roll)") FFixed Roll, UPARAM(DisplayName = "Y (Pitch)") FFixed Pitch, UPARAM(DisplayName = "Z (Yaw)") FFixed Yaw) { return FFixedRotator(Pitch, Yaw, Roll); }

	UFUNCTION(BlueprintPure, Category = "Math|FixedRotator", meta = (Keywords = "rotation rotate rotator breakrotator", NativeBreakFunc))
	static void BreakFixedRotator(UPARAM(DisplayName = "Rotation") const FFixedRotator& InRot, UPARAM(DisplayName = "X (Roll)") FFixed& Roll, UPARAM(DisplayName = "Y (Pitch)") FFixed& Pitch, UPARAM(DisplayName = "Z (Yaw)") FFixed& Yaw) { Roll = InRot.Roll; Pitch = InRot.Pitch; Yaw = InRot.Yaw; }

	// 类型转换

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFixedRotator (Rotator)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FFixedRotator Conv_RotatorToFixedRotator(const FRotator& InRotator) { return static_cast<FFixedRotator>(InRotator); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRotator (FixedRotator)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "Math|Conversions")
	static FORCEINLINE FRotator Conv_FixedRotatorToRotator(const FFixedRotator& InRotator) { return FRotator(InRotator); }

	// 布尔运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (FixedRotator)", CompactNodeTitle = "==", ScriptMethod = "IsNearEqual", ScriptOperator = "==", Keywords = "== equal"), Category = "Math|FixedRotator")
	static FORCEINLINE bool EqualEqual_FixedRotatorFixedRotator(const FFixedRotator& A, const FFixedRotator& B) { return A == B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Not Equal (FixedRotator)", CompactNodeTitle = "!=", ScriptMethod = "IsNotNearEqual", ScriptOperator = "!=", Keywords = "!= not equal"), Category = "Math|FixedRotator")
	static FORCEINLINE bool NotEqual_FixedRotatorFixedRotator(const FFixedRotator& A, const FFixedRotator& B) { return A != B; }

	// 旋转运算

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Normalize (FixedRotator)", ScriptMethod = "Normalize", Keywords = "Unit Rotator"), Category = "Math|FixedRotator")
	static FORCEINLINE FFixedRotator Normalize_FixedRotator(const FFixedRotator& A) { return FFixedMath::NormalizeRotator(A); }

//	UFUNCTION(BlueprintPure, meta = (DisplayName = "CombineFixedRotators", ScriptMethod = "Combine", Keywords = "rotate rotation add"), Category = "Math|FixedRotator")
//	static FORCEINLINE FFixedRotator CombineFixedRotators(const FFixedRotator& A, const FFixedRotator& B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Delta (FixedRotator)", ScriptMethod = "Delta"), Category = "Math|FixedRotator")
	static FORCEINLINE FFixedRotator NormalizedDeltaFixedRotator(const FFixedRotator& A, const FFixedRotator& B) { return FFixedMath::NormalizeRotator(A - B); }

//	UFUNCTION(BlueprintPure, meta = (DisplayName = "InvertFixedRotator", ScriptMethod = "Inversed", ScriptOperator = "neg", Keywords = "rotate rotation"), Category = "Math|FixedRotator")
//	static FORCEINLINE FFixedRotator NegateFixedRotator(const FFixedRotator& A);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ScaleFixedRotator", CompactNodeTitle = "*", ScriptMethod = "Scale", Keywords = "* multiply rotate rotation"), Category = "Math|FixedRotator")
	static FORCEINLINE FFixedRotator Multiply_FixedRotatorFixed(const FFixedRotator& A, FFixed B) { return A * B; }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Lerp (FixedRotator)", ScriptMethod = "Lerp"), Category = "Math|FixedRotator")
	static FORCEINLINE FFixedRotator Lerp_FixedRotator(const FFixedRotator& A, const FFixedRotator& B, FFixed Alpha) { return FFixedMath::LerpRotator(A, B, Alpha); }
};
