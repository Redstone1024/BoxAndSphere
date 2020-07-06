// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugActor.h"
#include "DrawDebugHelpers.h"

ADebugActor::ADebugActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Navigation.Resize({ 16, 16, 1 });

	Navigation[{ 8, 5, 0 }].CollisionFlags = 0;
	Navigation[{ 8, 6, 0 }].CollisionFlags = 0;
	Navigation[{ 8, 7, 0 }].CollisionFlags = 0;
	Navigation[{ 8, 8, 0 }].CollisionFlags = 0;
	Navigation[{ 8, 9, 0 }].CollisionFlags = 0;
	Navigation[{ 8, 10, 0 }].CollisionFlags = 0;

	Navigation[{ 7, 5, 0 }].CollisionFlags = 0;
	Navigation[{ 7, 6, 0 }].CollisionFlags = 0;
	Navigation[{ 7, 7, 0 }].CollisionFlags = 0;
	Navigation[{ 7, 8, 0 }].CollisionFlags = 0;
	Navigation[{ 7, 9, 0 }].CollisionFlags = 0;
	Navigation[{ 7, 10, 0 }].CollisionFlags = 0;

	Query.StartLocation = { 5, 6, 0 };
	Query.TargetLocation = { 10, 9, 0 };
	Query.AxisWeight = { 1024, 1024, 0 };
	Query.ColFlagsTrueMark = ~1;
	Query.ColFlagsFalseMark = 1;
}

void ADebugActor::BeginPlay()
{
	Super::BeginPlay();

	Navigation.FindPathSimple(Query, Path);
}

void ADebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugBox(GetWorld(), { Query.StartLocation.X * 100.0f, Query.StartLocation.Y * 100.0f, Query.StartLocation.Z * 100.0f }, { 25.0f, 25.0f, 25.0f }, FColor::Purple);
	DrawDebugBox(GetWorld(), { Query.TargetLocation.X * 100.0f, Query.TargetLocation.Y * 100.0f, Query.TargetLocation.Z * 100.0f }, { 25.0f, 25.0f, 25.0f }, FColor::Yellow);
	
	for (int i = 0; i < Navigation.GetSize().X; ++i)
	{
		for (int j = 0; j < Navigation.GetSize().Y; ++j)
		{
			for (int k = 0; k < Navigation.GetSize().Z; ++k)
			{
				FColor DrawColor = Navigation[{ i, j, k }].CollisionFlags & 1 ? FColor::Red : FColor::Green;
				DrawDebugBox(GetWorld(), { i * 100.0f, j * 100.0f, k * 100.0f }, { 49.0f, 49.0f, 49.0f }, DrawColor);
			}
		}
	}
	
	for (FIntVector& PothPoint : Path.LocationPath)
		DrawDebugBox(GetWorld(), { PothPoint.X * 100.0f, PothPoint.Y * 100.0f, PothPoint.Z * 100.0f }, { 40.0f, 40.0f, 40.0f }, FColor::Blue);

	for (int i = 0; i < Navigation.GetSize().X; ++i)
	{
		for (int j = 0; j < Navigation.GetSize().Y; ++j)
		{
			for (int k = 0; k < Navigation.GetSize().Z; ++k)
			{
				if (Path.Visited[FCheckerNavigation::IntVectorToIndex(Navigation.GetSize(), { i, j, k })])
				{
					DrawDebugBox(GetWorld(), { i * 100.0f, j * 100.0f, k * 100.0f }, { 45.0f, 45.0f, 45.0f }, FColor::Orange);
				}
			}
		}
	}

}

float ADebugActor::UpdateFindPath()
{
	Navigation.FindPathSimple(Query, Path);
	return Path.Timespan * 1000;
}

