// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridAgentComponent.generated.h"

class USmallGridMapSubsystem;

// 移动受阻时
UENUM(BlueprintType)
enum class EGridAgentClashMode : uint8
{
	Initiative, // 使对方离开世界
	Passive,    // 不移动
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridAgentInside, FIntVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridAgentOutside);

UCLASS(ClassGroup = "SmallGridMap", HideCategories = ("Activation", "Collision", "ComponentReplication"), meta = (BlueprintSpawnableComponent))
class SMALLGRIDMAP_API UGridAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGridAgentComponent();

	UPROPERTY(BlueprintAssignable, Category = "Small Grid Map")
	FGridAgentInside OnInside;

	UPROPERTY(BlueprintAssignable, Category = "Small Grid Map")
	FGridAgentOutside OnOutside;

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	FIntVector GetGridLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Small Grid Map")
	bool SetGridLocation(FIntVector InLocation, EGridAgentClashMode ClashMode = EGridAgentClashMode::Passive, bool bOnlyTrace = false);

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	int64 GetCollisionFlags() const;

	UFUNCTION(BlueprintCallable, Category = "Small Grid Map")
	bool SetCollisionFlags(int64 InCollisionFlags);

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	const TArray<FIntVector>& GetCollisionRange() const;

	UFUNCTION(BlueprintCallable, Category = "Small Grid Map")
	bool SetCollisionRange(const TArray<FIntVector>& InCollisionRange, EGridAgentClashMode ClashMode = EGridAgentClashMode::Passive, bool bOnlyTrace = false);

private:
	UPROPERTY() USmallGridMapSubsystem* GridSubsystem;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	FIntVector GridLocation;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	int64 CollisionFlags;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	TArray<FIntVector> CollisionRange;

public:
	//~ Begin USubsystem Interface
	virtual void BeginPlay() override;
	//~ End USubsystem Interface
};
