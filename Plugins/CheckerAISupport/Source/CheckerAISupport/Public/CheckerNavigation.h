// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class CHECKERAISUPPORT_API FCheckerNavigation
{
public:

	struct FBlock
	{
		int32 CollisionFlags = 1;
		//	int32 WeightsFlags = 0;
	};

	struct FAgent
	{

	};

	struct FPathFindingQuery
	{
		FIntVector StartLocation = { 0, 0, 0 };
		FIntVector TargetLocation = { 0, 0, 0 };
		
		FIntVector AxisWeight = { 1024, 1024, 1024 };

		int32 ColFlagsTrueMark = -1;
		int32 ColFlagsFalseMark = 0;

		TArray<FIntVector> NeighborsDirection =
		{
			{ -1,  0,  0 }, {  0, -1,  0 }, {  0,  0, -1 }, {  0,  0,  1 }, {  0,  1,  0 }, {  1,  0,  0 }, { -1, -1,  0 },
			{ -1,  0, -1 }, { -1,  0,  1 }, { -1,  1,  0 }, {  0, -1, -1 }, {  0, -1,  1 }, {  0,  1, -1 }, {  0,  1,  1 },
			{  1, -1,  0 }, {  1,  0, -1 }, {  1,  0,  1 }, {  1,  1,  0 }, { -1, -1, -1 }, { -1, -1,  1 }, { -1,  1, -1 },
			{ -1,  1,  1 }, {  1, -1, -1 }, {  1, -1,  1 }, {  1,  1, -1 }, {  1,  1,  1 },
		};
	};

	struct FPathResult
	{
		bool bSuccess = false;
		TArray<FIntVector> LocationPath;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		TArray<bool> Visited;
		double Timespan;
#endif
	};

private:

	constexpr static int32 MaxBlockDataSize = 1024 * 1024 * 1024;

	FIntVector Size = { 0, 0, 0 };
	TArray<FBlock> BlockData;

private:

	struct FComputeBuffer;
	TSharedPtr<FComputeBuffer> DefaultComputeBuffer;

public:

	FCheckerNavigation() : DefaultComputeBuffer(CreateComputeBuffer()) { }
	FCheckerNavigation(FIntVector InSize) : FCheckerNavigation() { Resize(InSize); }

	FORCEINLINE const FIntVector& GetSize() const { return Size; }
	FIntVector Resize(const FIntVector& NewSize);

	static FORCEINLINE bool IntVectorIsIndex(const FIntVector& InSize, const FIntVector& InLocation);
	static FORCEINLINE int32 IntVectorToIndex(const FIntVector& InSize, const FIntVector& InLocation);

	FORCEINLINE FBlock& operator[](const FIntVector& Location);
	FORCEINLINE const FBlock& operator[](const FIntVector& Location) const;

	TSharedPtr<FComputeBuffer> CreateComputeBuffer() const;
	bool FindPathSimple(const FPathFindingQuery& Query, FPathResult& Result) { return FindPathSimple(Query, Result, DefaultComputeBuffer); }
	bool FindPathSimple(const FPathFindingQuery& Query, FPathResult& Result, TSharedPtr<FComputeBuffer> ComputeBuffer /*= DefaultComputeBuffer*/) const;
};

FORCEINLINE bool FCheckerNavigation::IntVectorIsIndex(const FIntVector & InSize, const FIntVector & InLocation)
{
	return InLocation.X < InSize.X && InLocation.Y < InSize.Y && InLocation.Z < InSize.Z
		&& InLocation.X >= 0 && InLocation.Y >= 0 && InLocation.Z >= 0;
}

int32 FORCEINLINE FCheckerNavigation::IntVectorToIndex(const FIntVector & InSize, const FIntVector & InLocation)
{
	check(IntVectorIsIndex(InSize, InLocation));
	return InLocation.X + InSize.X * InLocation.Y + InSize.X * InSize.Y * InLocation.Z;
}

FORCEINLINE FCheckerNavigation::FBlock& FCheckerNavigation::operator[](const FIntVector& Location)
{
	return BlockData[IntVectorToIndex(Size, Location)];
}

FORCEINLINE const FCheckerNavigation::FBlock& FCheckerNavigation::operator[](const FIntVector& Location) const
{
	return BlockData[IntVectorToIndex(Size, Location)];
}
