// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>

#include "CoreMinimal.h"
#include "GridPathFinder.h"
#include "AStarGridPathFinder.generated.h"

class FAStarGridPathFinder;

UCLASS(BlueprintInternalUseOnly)
class SMALLGRIDMAP_API UAStarGridPathFinder : public UGridPathFinder
{
	GENERATED_BODY()

public:
	UAStarGridPathFinder(const FObjectInitializer& ObjectInitializer);

	virtual bool FindPath(UGridAgentComponent* Agent, FIntVector Target, FGridPathFindingResult& Result) final;
	
private:
	UPROPERTY() USmallGridMapSubsystem* GridSubsystem;

	struct FOpenPoint
	{
		FIntVector Location;

		FFixed Cost;
		FFixed Pred;

		FOpenPoint* Father;
	};

	struct FOpenPointCompare
	{
		FORCEINLINE bool operator()(const FOpenPoint* A, const FOpenPoint* B)
		{
			return A->Pred > B->Pred;
		}
	};

	const static int32 DepthLimit = 16384;
	const static int32 DirectionNum = 27;
	const static FIntVector Direction[DirectionNum];

	int32 CloseNumber;
	TArray<int32> CloseList;
	TArray<FOpenPoint> PointList;
	std::priority_queue<FOpenPoint*, std::vector<FOpenPoint*>, FOpenPointCompare> OpenList;
};
