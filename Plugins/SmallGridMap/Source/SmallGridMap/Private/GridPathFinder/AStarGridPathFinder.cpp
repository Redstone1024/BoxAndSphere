// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathFinder/AStarGridPathFinder.h"

#include "GridAgentComponent.h"
#include "SmallGridMapSubsystem.h"

const FIntVector UAStarGridPathFinder::Direction[] = 
{
	{ -1, -1, -1 }, { -1, -1,  0 }, { -1, -1,  1 },
	{ -1,  0, -1 }, { -1,  0,  0 }, { -1,  0,  1 },
	{ -1,  1, -1 }, { -1,  1,  0 }, { -1,  1,  1 },
	{  0, -1, -1 }, {  0, -1,  0 }, {  0, -1,  1 },
	{  0,  0, -1 },                 {  0,  0,  1 },
	{  0,  1, -1 }, {  0,  1,  0 }, {  0,  1,  1 },
	{  1, -1, -1 }, {  1, -1,  0 }, {  1, -1,  1 },
	{  1,  0, -1 }, {  1,  0,  0 }, {  1,  0,  1 },
	{  1,  1, -1 }, {  1,  1,  0 }, {  1,  1,  1 }
};

UAStarGridPathFinder::UAStarGridPathFinder(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CloseNumber(1)
{
	if (IsTemplate()) return;

	verify((GridSubsystem = Cast<USmallGridMapSubsystem>(GetOuter())) != nullptr);

	PointList.Reserve(DepthLimit + DirectionNum);
	CloseList.Init(CloseNumber - 1, GridSubsystem->GetSize().X *  GridSubsystem->GetSize().Y * GridSubsystem->GetSize().Z);
}

bool UAStarGridPathFinder::FindPath(UGridAgentComponent * Agent, FIntVector Target, FGridPathFindingResult & Result)
{
	check(GridSubsystem);

	QUICK_SCOPE_CYCLE_COUNTER(STAT_AStarGridPathFinder_FindPath);

	if (!GridSubsystem->IsValidLocation(Agent->GetGridLocation())
		|| !GridSubsystem->IsValidLocation(Target)) return false;

	Result.bSuccess = false;
	Result.Length = -1;
	Result.Path.SetNum(0, false);
	Result.Barriers.SetNum(0, false);

	PointList.SetNum(0, false);

	OpenList.push(&PointList[PointList.Add({ Agent->GetGridLocation(), 0, 0, nullptr })]);

	int32 Temp;
	FIntVector Neighbor;
	FIntVector Relative;
	FOpenPoint* ToOpen = nullptr;

	while (!OpenList.empty())
	{
		ToOpen = OpenList.top();
		OpenList.pop();

		if (ToOpen->Location == Target)
		{
			Result.bSuccess = true;
			break;
		}
		if (PointList.Num() >= DepthLimit)
		{
			ToOpen = nullptr;
			break;
		}

		if (CloseList[GridSubsystem->GetLocationIndex(ToOpen->Location)] == CloseNumber) continue;
		CloseList[GridSubsystem->GetLocationIndex(ToOpen->Location)] = CloseNumber;

		for (int i = 0; i < DirectionNum; ++i)
		{
			Neighbor = ToOpen->Location + Direction[i];

			if (!GridSubsystem->IsValidLocation(Neighbor)) continue;
			if (CloseList[GridSubsystem->GetLocationIndex(Neighbor)] == CloseNumber) continue;
			if ((GridSubsystem->GetStaticCollisionFlags(Neighbor)
				| GridSubsystem->GetDynamicCollisionFlags(Neighbor))
				& Agent->GetCollisionFlags())
			{
				Result.Barriers.Add(Neighbor);
				continue;
			}

			int Cost = (Direction[i].X != 0) + (Direction[i].Y != 0) + (Direction[i].Z != 0);
			Cost = Cost == 3 ? 17 : Cost == 2 ? 14 : 10;
			Cost += ToOpen->Cost;

			FOpenPoint* NewPoint = &PointList[PointList.Add({ Neighbor, Cost, 0, ToOpen })];

			Relative.X = FMath::Abs(Target.X - Neighbor.X);
			Relative.Y = FMath::Abs(Target.Y - Neighbor.Y);
			Relative.Z = FMath::Abs(Target.Z - Neighbor.Z);

			if (Relative.X > Relative.Y)
			{
				Temp = Relative.X;
				Relative.X = Relative.Y;
				Relative.Y = Temp;
			}
			if (Relative.Y > Relative.Z)
			{
				Temp = Relative.Y;
				Relative.Y = Relative.Z;
				Relative.Z = Temp;
			}
			if (Relative.X > Relative.Y)
			{
				Temp = Relative.X;
				Relative.X = Relative.Y;
				Relative.Y = Temp;
			}

			NewPoint->Pred = Relative.X * 3 + Relative.Y * 4 + Relative.Z * 10 + Cost;

			OpenList.push(NewPoint);
		}
	}

	if (Result.bSuccess)
	{
		Result.Length = ToOpen->Cost;

		for (FOpenPoint* RS = ToOpen; RS; RS = RS->Father)
			Result.Path.Add(RS->Location);
	}

	++CloseNumber;
	while (!OpenList.empty()) OpenList.pop();

	return Result.bSuccess;
}
