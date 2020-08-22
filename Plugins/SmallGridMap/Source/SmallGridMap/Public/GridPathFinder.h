// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridPathFinder.generated.h"

class UGridAgentComponent;
class USmallGridMapSubsystem;

USTRUCT(BlueprintType)
struct FGridPathFindingResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Small Grid Map")
	bool bSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "Small Grid Map")
	int32 Length;

	UPROPERTY(BlueprintReadOnly, Category = "Small Grid Map")
	TArray<FIntVector> Path;
};

UCLASS(Abstract, Within = "SmallGridMapSubsystem")
class SMALLGRIDMAP_API UGridPathFinder : public UObject
{
	GENERATED_BODY()

	friend USmallGridMapSubsystem;

protected:
	UFUNCTION(BlueprintCallable, Category = "Small Grid Map", BlueprintPure = false)
	virtual bool FindPath(UGridAgentComponent* Agent, FIntVector Target, FGridPathFindingResult& Result) PURE_VIRTUAL(UGridPathFinder::FindPath, return false;);
};
