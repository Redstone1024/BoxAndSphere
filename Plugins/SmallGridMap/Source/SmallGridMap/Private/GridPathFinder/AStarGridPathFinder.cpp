// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathFinder/AStarGridPathFinder.h"

#include "GridAgentComponent.h"
#include "SmallGridMapSubsystem.h"

class FAStarGridPathFinder
{
	friend UAStarGridPathFinder;

	//是否在障碍物或者关闭列表
	static FORCEINLINE bool InBarrierOrCloseList(UAStarGridPathFinder* Finder, const FIntVector& A, UGridAgentComponent* Agent) {
		if (!Finder->GridSubsystem->IsValidLocation(A)) return true;
		if ((Finder->GridSubsystem->GetStaticCollisionFlags(A) | Finder->GridSubsystem->GetDynamicCollisionFlags(A)) & Agent->GetCollisionFlags()) return true;
		return Finder->CloseList[Finder->GridSubsystem->GetLocationIndex(A)] == Finder->CloseNumber;
	}

	//创建一个开启点
	static FORCEINLINE UAStarGridPathFinder::FOpenPoint* CreateOpenPoint(UAStarGridPathFinder* Finder, const FIntVector& A, const FIntVector& Target, int32 Cost, UAStarGridPathFinder::FOpenPoint* Father)
	{
		UAStarGridPathFinder::FOpenPoint& Result =
			Finder->PointList[Finder->PointList.Add(UAStarGridPathFinder::FOpenPoint())];

		Result.Location = A;
		Result.Cost = Cost;
		Result.Father = Father;

		FIntVector Relative;
		Relative.X = FMath::Abs(Target.X - A.X);
		Relative.Y = FMath::Abs(Target.Y - A.Y);
		Relative.Z = FMath::Abs(Target.Z - A.Z);

		int32 Temp;

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

		Result.Pred = Relative.X * 3 + Relative.Y * 4 + Relative.Z * 10 + Cost;

		return &Result;
	}

	// 开启检查，检查父节点
	static FORCEINLINE void Open(UAStarGridPathFinder* Finder, UAStarGridPathFinder::FOpenPoint* PointToOpen, const FIntVector& Target, UGridAgentComponent* Agent)
	{
		if (InBarrierOrCloseList(Finder, PointToOpen->Location, Agent)) return;

		for (int i = 0; i < UAStarGridPathFinder::DirectionNum; ++i)
		{
			FIntVector Neighbor = PointToOpen->Location + UAStarGridPathFinder::Direction[i];
			if (!InBarrierOrCloseList(Finder, Neighbor, Agent))
			{
				int Cost =
					(UAStarGridPathFinder::Direction[i].X != 0) +
					(UAStarGridPathFinder::Direction[i].Y != 0) +
					(UAStarGridPathFinder::Direction[i].Z != 0);
				Cost = Cost == 3 ? 17 : Cost == 2 ? 14 : 10;
				Finder->OpenList.push(CreateOpenPoint(Finder, Neighbor, Target, PointToOpen->Cost + Cost, PointToOpen));
			}
		}

		Finder->CloseList[Finder->GridSubsystem->GetLocationIndex(PointToOpen->Location)] = Finder->CloseNumber;
	}
};

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

	PointList.SetNum(0, false);

	OpenList.push(FAStarGridPathFinder::CreateOpenPoint(this, Agent->GetGridLocation(), Target, 0, nullptr));

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

		FAStarGridPathFinder::Open(this, ToOpen, Target, Agent);
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
