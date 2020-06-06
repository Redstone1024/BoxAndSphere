// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define INT16TOBYTES(x) (uint8)((x) >>  0),\
						(uint8)((x) >>  8)

#define INT32TOBYTES(x) (uint8)((x) >>  0),\
						(uint8)((x) >>  8),\
						(uint8)((x) >> 16),\
						(uint8)((x) >> 24)


#define INT64TOBYTES(x) (uint8)((x) >>  0),\
						(uint8)((x) >>  8),\
						(uint8)((x) >> 16),\
						(uint8)((x) >> 24),\
						(uint8)((x) >> 32),\
						(uint8)((x) >> 40),\
						(uint8)((x) >> 48),\
						(uint8)((x) >> 56)

#define BYTESTOINT16(x) ((uint16)(*((x) + 0)) <<  0) + \
						((uint16)(*((x) + 1)) <<  8)

#define BYTESTOINT32(x) ((uint32)(*((x) + 0)) <<  0) + \
						((uint32)(*((x) + 1)) <<  8) + \
						((uint32)(*((x) + 2)) << 16) + \
						((uint32)(*((x) + 3)) << 24)

#define BYTESTOINT64(x) ((uint64)(*((x) + 0)) <<  0) + \
						((uint64)(*((x) + 1)) <<  8) + \
						((uint64)(*((x) + 2)) << 16) + \
						((uint64)(*((x) + 3)) << 24) + \
						((uint64)(*((x) + 4)) << 32) + \
						((uint64)(*((x) + 5)) << 40) + \
						((uint64)(*((x) + 6)) << 48) + \
						((uint64)(*((x) + 7)) << 56)

class FBytesHelper
{
public:
	inline static uint8 ComputeCheck(const TArray<uint8>& Data, int32 BeginIndex = 0, int32 EndIndex = INT32_MAX)
	{
		BeginIndex = FMath::Max(0, BeginIndex);
		EndIndex = FMath::Min(Data.Num(), EndIndex);

		uint8 Check = 0;
		for (int32 Index = BeginIndex; Index < EndIndex; Index++)
			Check ^= Data[Index];
		return Check;
	}
};
