// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallGridMapConfig.h"

#include "Misc/Base64.h"

const FString USmallGridMapConfig::ConfigAssetSuffix = TEXT("_GridConfig");

USmallGridMapConfig::USmallGridMapConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StaticCollisionFlags.Init(0, 16 * 16 * 1);
}

void USmallGridMapConfig::UpdateSetting()
{
	if (LoadUpdateData == TEXT(""))
	{
		Size = FIntVector(16, 16, 1); 
		StaticCollisionFlags.Init(0, 16 * 16 * 1);
		LoadInformation = TEXT("Settings are set to default.");
	}

	int32 Index = 0;
	TArray<uint8> DecodeData;
	FBase64::Decode(LoadUpdateData, DecodeData);
	
	do 
	{
		FIntVector UpdateSize;
		
		if (DecodeData.Num() < 12) break;

		UpdateSize.X = ((int32)DecodeData[0] << 24) + ((int32)DecodeData[1] << 16) + ((int32)DecodeData[2] << 8) + ((int32)DecodeData[3] << 0);
		UpdateSize.Y = ((int32)DecodeData[4] << 24) + ((int32)DecodeData[5] << 16) + ((int32)DecodeData[6] << 8) + ((int32)DecodeData[7] << 0);
		UpdateSize.Z = ((int32)DecodeData[8] << 24) + ((int32)DecodeData[9] << 16) + ((int32)DecodeData[10] << 8) + ((int32)DecodeData[11] << 0);

		if (UpdateSize.X < MinSideLength || UpdateSize.X > MaxSideLength) break;
		if (UpdateSize.Y < MinSideLength || UpdateSize.Y > MaxSideLength) break;
		if (UpdateSize.Z < MinSideLength || UpdateSize.Z > MaxSideLength) break;
		
		if (1ll * Size.X * Size.Y * Size.Z > MaxGridNum) break;

		Index = 12;

		TArray<int64> UpdateCollisionFlags;
		UpdateCollisionFlags.SetNum(UpdateSize.X * UpdateSize.Y * UpdateSize.Z);

		for (int32 i = 0; i < UpdateCollisionFlags.Num(); ++i)
		{
			if (Index > DecodeData.Num() - 8)
			{
				Index = -1;
				break;
			}

			UpdateCollisionFlags[i] =
				((int64)DecodeData[Index + 0] << 56) +
				((int64)DecodeData[Index + 1] << 48) +
				((int64)DecodeData[Index + 2] << 40) +
				((int64)DecodeData[Index + 3] << 32) +
				((int64)DecodeData[Index + 4] << 24) +
				((int64)DecodeData[Index + 5] << 16) +
				((int64)DecodeData[Index + 6] << 8) +
				((int64)DecodeData[Index + 7] << 0);

			Index += 8;
		}

		if (Index == -1) break;
		if (Index != DecodeData.Num()) break;

		Modify(false);
		Size = UpdateSize;
		LoadUpdateData = TEXT("");
		StaticCollisionFlags = UpdateCollisionFlags;

		LoadInformation = TEXT("Successfully Loaded!");
		return;
	} 
	while (false);

	LoadInformation = TEXT("Load Failed To Format Error.");
}
