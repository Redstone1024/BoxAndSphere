// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SmallGridMapConfig.generated.h"

UCLASS()
class SMALLGRIDMAP_API USmallGridMapConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	static constexpr int32 MinSideLength = 1;
	static constexpr int32 MaxSideLength = 8192;
	static constexpr int32 MaxGridNum = 8192 * 8192;
	static const FString ConfigAssetSuffix;

	USmallGridMapConfig(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = "Current Setting")
	FIntVector Size = FIntVector(16, 16, 1);

	UPROPERTY()
	TArray<int64> StaticCollisionFlags;

	UFUNCTION(CallInEditor, Category = "Load Setting")
	void UpdateSetting();

	UPROPERTY(EditAnywhere, Category = "Load Setting", Transient)
	FString LoadUpdateData;

	UPROPERTY(VisibleAnywhere, Category = "Load Setting", Transient)
	FString LoadInformation;
};
