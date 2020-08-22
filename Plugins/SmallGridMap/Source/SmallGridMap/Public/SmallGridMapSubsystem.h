// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SmallGridMapSubsystem.generated.h"

class UGridPathFinder;
class UGridAgentComponent;

USTRUCT(BlueprintType)
struct FGridMapNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Small Grid Map")
	int64 StaticCollisionFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Small Grid Map")
	int64 DynamicCollisionFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Small Grid Map")
	TWeakObjectPtr<UGridAgentComponent> Agent;

	FGridMapNode() = default;
	FGridMapNode(int64 InStaticCollisionFlags, int64 InDynamicCollisionFlags, TWeakObjectPtr<UGridAgentComponent> InAgent)
		: StaticCollisionFlags(InStaticCollisionFlags)
		, DynamicCollisionFlags(InDynamicCollisionFlags)
		, Agent(InAgent)
	{ }
};

UCLASS()
class SMALLGRIDMAP_API USmallGridMapSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	friend UGridAgentComponent;

public:
	USmallGridMapSubsystem(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FORCEINLINE FIntVector GetSize() const { return Size; }

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FORCEINLINE bool IsValidLocation(FIntVector Location) const;

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FORCEINLINE int32 GetLocationIndex(FIntVector Location) const;

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FORCEINLINE FGridMapNode GetNode(FIntVector Location) const;

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FORCEINLINE int64 GetStaticCollisionFlags(FIntVector Location) const
	{ return IsValidLocation(Location) ? StaticCollisionFlags[GetLocationIndex(Location)] : OutsideNode.StaticCollisionFlags; }

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FORCEINLINE int64 GetDynamicCollisionFlags(FIntVector Location) const
	{ return IsValidLocation(Location) ? DynamicCollisionFlags[GetLocationIndex(Location)] : OutsideNode.DynamicCollisionFlags; }

	UFUNCTION(BlueprintCallable, Category = "Small Grid Map")
	void SetDynamicCollisionFlags(FIntVector Location, int64 CollisionFlags)
	{ if (IsValidLocation(Location)) DynamicCollisionFlags[GetLocationIndex(Location)] = CollisionFlags; }

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	UGridPathFinder* GetPathFinder() const { return PathFinder; }

private:
	UPROPERTY() FIntVector Size;
	UPROPERTY() FGridMapNode OutsideNode;
	UPROPERTY() TArray<int64> StaticCollisionFlags;
	UPROPERTY() TArray<int64> DynamicCollisionFlags;
	UPROPERTY() TArray<TWeakObjectPtr<UGridAgentComponent>> Agents;

	UPROPERTY() UGridPathFinder* PathFinder;

public:
	//~ Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override { }
	//~ End USubsystem Interface
};

bool USmallGridMapSubsystem::IsValidLocation(FIntVector Location) const
{
	return Location.X >= 0 && Location.X < Size.X
		&& Location.Y >= 0 && Location.Y < Size.Y
		&& Location.Z >= 0 && Location.Z < Size.Z;
}

int32 USmallGridMapSubsystem::GetLocationIndex(FIntVector Location) const
{
	check(IsValidLocation(Location));
	return Location.X * Size.Y * Size.Z + Location.Y * Size.Z + Location.Z;
}

FGridMapNode USmallGridMapSubsystem::GetNode(FIntVector Location) const
{
	return IsValidLocation(Location) ?
		FGridMapNode
		(
			GetStaticCollisionFlags(Location),
			GetDynamicCollisionFlags(Location),
			Agents[GetLocationIndex(Location)]
		)
		: OutsideNode;
}
