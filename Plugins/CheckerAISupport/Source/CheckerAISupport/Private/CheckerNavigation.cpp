// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckerNavigation.h"
#include "Containers/Queue.h"
#include "Algo/FindSequence.h"

#include <queue>

namespace
{
	struct FBlockNode
	{
		bool bOpened = false;
		bool bClosed = false;
		unsigned int H = 0;
		unsigned int G = UINT_MAX;
		FIntVector LastLocation = FIntVector(-1, -1, -1);
	};

	struct FOpenListNode
	{
		FIntVector Location;
		unsigned int F = 0;

		FORCEINLINE FOpenListNode(const FIntVector& InLocation, unsigned int InF)
			: Location(InLocation)
			, F(InF)
		{ }

		friend FORCEINLINE bool operator <(const FOpenListNode& LHS, const FOpenListNode& RHS) { return LHS.F < RHS.F; }
		friend FORCEINLINE bool operator >(const FOpenListNode& LHS, const FOpenListNode& RHS) { return LHS.F > RHS.F; }
	};

	typedef std::priority_queue<FOpenListNode, std::vector<FOpenListNode>, std::greater<FOpenListNode>> FOpenListType;
}

struct FCheckerNavigation::FComputeBuffer
{
	FOpenListType OpenList;
	TArray<FBlockNode> BlockData;
	TArray<FIntVector> Changed;
};

FIntVector FCheckerNavigation::Resize(const FIntVector & NewSize)
{
	check(NewSize.X * NewSize.Y * NewSize.Z <= MaxBlockDataSize)

	FIntVector OldSize = Size;
	TArray<FBlock> OldBlockData = BlockData;

	Size = NewSize;
	if (Size.X <= 0 || Size.Y <= 0 || Size.Z <= 0)
		Size = { 0, 0, 0 };

	BlockData.Init(FBlock(), Size.X * Size.Y * Size.Z);

	if (!OldSize.IsZero() && !Size.IsZero())
	{
		for (int i = 0; i < FMath::Min(OldSize.X, Size.X); ++i)
			for (int j = 0; j < FMath::Min(OldSize.Y, Size.Y); ++j)
				for (int k = 0; k < FMath::Min(OldSize.Z, Size.Z); ++k)
					(*this)[{ i, j, k }] = OldBlockData[IntVectorToIndex(OldSize, { i, j, k })];
	}

	DefaultComputeBuffer = CreateComputeBuffer();

	return Size;
}

TSharedPtr<FCheckerNavigation::FComputeBuffer> FCheckerNavigation::CreateComputeBuffer() const
{
	TSharedPtr<FComputeBuffer> NewComputeBuffer(new FComputeBuffer());
	NewComputeBuffer->BlockData.Init(FBlockNode(), Size.X * Size.Y * Size.Z);
	return NewComputeBuffer;
}

bool FCheckerNavigation::FindPathSimple(const FPathFindingQuery & Query, FPathResult & Result, TSharedPtr<FComputeBuffer> ComputeBuffer) const
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	double BeginTime = FPlatformTime::Seconds();
	Result.Visited.Init(false, Size.X * Size.Y * Size.Z);
#endif

	if (!ComputeBuffer)
		ComputeBuffer = CreateComputeBuffer();

	Result.bSuccess = false;
	Result.LocationPath.SetNum(0, false);

	TArray<FBlockNode>& Nodes = ComputeBuffer->BlockData;
	FOpenListType& OpenList = ComputeBuffer->OpenList;
	TArray<FIntVector>& Changed = ComputeBuffer->Changed;

#define NODES(Index) Nodes[IntVectorToIndex(Size, Index)]

	TArray<unsigned int> NDG;
	NDG.Init(0, Query.NeighborsDirection.Num());
	for (int32 i = 0; i < Query.NeighborsDirection.Num(); i++)
		NDG[i] = FVector(
			Query.NeighborsDirection[i].X * Query.AxisWeight.X,
			Query.NeighborsDirection[i].Y * Query.AxisWeight.Y,
			Query.NeighborsDirection[i].Z * Query.AxisWeight.Z
		).Size();

	Changed.Add(Query.StartLocation);
	NODES(Query.StartLocation).G = 0;
	NODES(Query.StartLocation).bOpened = true;
	OpenList.push(FOpenListNode(Query.StartLocation, 0));

	unsigned int NG;
	FIntVector Node;
	FIntVector Neighbor;
	while (!OpenList.empty())
	{
		Node = OpenList.top().Location;
		NODES(Node).bClosed = true;
		OpenList.pop();

		if (Node == Query.TargetLocation)
		{
			Result.bSuccess = true;
			break;
		}

		for (int32 i = 0; i < Query.NeighborsDirection.Num(); i++)
		{
			const FIntVector& NeighborDirection = Query.NeighborsDirection[i];
			Neighbor = Node + NeighborDirection;

			if (!IntVectorIsIndex(Size, Neighbor)) continue;
			if (NODES(Neighbor).bClosed) continue;
			if ((*this)[Neighbor].CollisionFlags & Query.ColFlagsTrueMark) continue;
			if (~(*this)[Neighbor].CollisionFlags & Query.ColFlagsFalseMark) continue;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			Result.Visited[IntVectorToIndex(Size, Neighbor)] = true;
#endif

			NG = NODES(Node).G + NDG[i];

			if (!NODES(Neighbor).bOpened || NG < NODES(Neighbor).G)
			{
				NODES(Neighbor).G = NG;

				if (!NODES(Neighbor).H)
					NODES(Neighbor).H = (
						  FMath::Abs(Neighbor.X - Query.TargetLocation.X) * Query.AxisWeight.X
						+ FMath::Abs(Neighbor.Y - Query.TargetLocation.Y) * Query.AxisWeight.Y
						+ FMath::Abs(Neighbor.Z - Query.TargetLocation.Z) * Query.AxisWeight.Z
					);

				NODES(Neighbor).LastLocation = Node;

				OpenList.push(FOpenListNode(Neighbor, NODES(Neighbor).G + NODES(Neighbor).H));
				if (!NODES(Neighbor).bOpened) Changed.Add(Neighbor);
				NODES(Neighbor).bOpened = true;
			}
		}
	}

	if (Result.bSuccess)
	{
		FIntVector Last = Query.TargetLocation;
		while (IntVectorIsIndex(Size, Last))
		{
			Result.LocationPath.Add(Last);
			Last = NODES(Last).LastLocation;
		}
		Algo::Reverse(Result.LocationPath);
	}

	for (FIntVector& ChangedNodes : Changed)
		NODES(ChangedNodes) = FBlockNode();
	while (!OpenList.empty()) OpenList.pop();
	Changed.SetNum(0, false);

#undef NODES

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	double EndTime = FPlatformTime::Seconds();
	Result.Timespan = EndTime - BeginTime;
#endif

	return Result.bSuccess;
}
