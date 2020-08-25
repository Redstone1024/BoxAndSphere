// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridAgentComponent.generated.h"

class USmallGridMapSubsystem;

// 移动受阻时
UENUM(BlueprintType)
enum class EGridAgentClashMode : uint8
{
	Initiative, // 使对方离开世界
	Passive,    // 不移动
};

UENUM(BlueprintType)
enum class EGridAgentDirection : uint8
{
	TN = 0,
	TS = 4,
	TW = 6,
	TE = 2,
	NW = 7,
	NE = 1,
	SW = 5,
	SE = 3,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridAgentInside, FIntVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGridAgentOutside);

UCLASS()
class SMALLGRIDMAP_API UGridAgentHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	static int32 GridDirectionToAngle(EGridAgentDirection InGridDirection) { return (int32)InGridDirection * 45; };

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	static EGridAgentDirection AngleToGridDirection(int32 InAngle) { InAngle %= 360; if (InAngle < 360) InAngle += 360; InAngle /= 45; return (EGridAgentDirection)InAngle; }
};

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

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	bool IsBlockPath(FIntVector Location) const;

	UFUNCTION(BlueprintPure, Category = "Small Grid Map")
	EGridAgentDirection GetGridDirection() const { return GridDirection; }

	UFUNCTION(BlueprintCallable, Category = "Small Grid Map")
	void SetGridDirection(EGridAgentDirection InGridDirection) { GridDirection = InGridDirection; }

private:
	UPROPERTY() USmallGridMapSubsystem* GridSubsystem;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	FIntVector GridLocation;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	int64 CollisionFlags;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	TArray<FIntVector> CollisionRange;

	UPROPERTY(EditAnywhere, Category = "Small Grid Map")
	EGridAgentDirection GridDirection;

public:
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	//~ End UActorComponent Interface
};
