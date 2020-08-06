// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFixed.h"

FFixed UBlueprintFixedMath::Divide_FixedFixed(FFixed A, FFixed B)
{
	if (B == 0)
	{
		FFrame::KismetExecutionMessage(TEXT("Divide by zero: Divide_FixedFixed"), ELogVerbosity::Warning, FName("DivideByZeroWarning"));
		return 0;
	}

	return A / B;
}

FFixed UBlueprintFixedMath::Sqrt_Fixed(FFixed A)
{
	if (A >= 0)
		return FFixedMath::Sqrt(A);

	FFrame::KismetExecutionMessage(TEXT("Attempt to take Sqrt_Fixed() of negative number - returning 0."), ELogVerbosity::Warning, FName("NegativeSqrtWarning"));
	return 0;
}

FFixedVector UBlueprintFixedMath::Divide_FixedVectorFixedVector(FFixedVector A, FFixedVector B)
{
	if (B == FFixedVector(0))
	{
		FFrame::KismetExecutionMessage(TEXT("Divide by zero: Divide_FixedFixed"), ELogVerbosity::Warning, FName("DivideByZeroWarning"));
		return FFixedVector(0);
	}

	return A / B;
}
