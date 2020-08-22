// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallGridMapSubsystem.h"

#include "Engine/World.h"
#include "GridPathFinder.h"
#include "SmallGridMapConfig.h"
#include "GridAgentComponent.h"
#include "GridPathFinder/AStarGridPathFinder.h"

namespace
{
	USmallGridMapConfig* LoadGridConfig(UWorld* World)
	{
		if (!World)
			return nullptr;

		FString GridConfigPath;
		World->GetPathName(nullptr, GridConfigPath);
		int32 PathLen = GridConfigPath.Len();
		while (PathLen > 0 && GridConfigPath[PathLen - 1] != TEXT('/')) --PathLen;
		GridConfigPath.RemoveAt(PathLen, GridConfigPath.Len() - PathLen, false);
		GridConfigPath.Append(World->GetName());
		GridConfigPath.Append(USmallGridMapConfig::ConfigAssetSuffix);

		return LoadObject<USmallGridMapConfig>(World, *GridConfigPath);
	}
}

USmallGridMapSubsystem::USmallGridMapSubsystem(const FObjectInitializer& ObjectInitializer)
{ }

bool USmallGridMapSubsystem::ShouldCreateSubsystem(UObject * Outer) const
{
	return LoadGridConfig(Cast<UWorld>(Outer));
}

void USmallGridMapSubsystem::Initialize(FSubsystemCollectionBase & Collection)
{
	USmallGridMapConfig* GridConfig = LoadGridConfig(GetWorld());

	check(GridConfig);

	Size = GridConfig->Size;
	StaticCollisionFlags = GridConfig->StaticCollisionFlags;

	OutsideNode.StaticCollisionFlags = -1;
	Agents.Init(nullptr, Size.X * Size.Y * Size.Z);
	DynamicCollisionFlags.Init(0, Size.X * Size.Y * Size.Z);

	PathFinder = NewObject<UGridPathFinder>(this, UAStarGridPathFinder::StaticClass());
}
