// Fill out your copyright notice in the Description page of Project Settings.


#include "LockstepParam.h"

#include "BytesHelper.h"

bool ULockstepParamInt32::FromBytes(const TArray<uint8>& Data)
{
	if (Data.Num() != 4)
		return false;

	Number = BYTESTOINT32(Data.GetData());

	return true;
}

bool ULockstepParamInt32::ToBytes(TArray<uint8>& Data)
{
	Data = { INT32TOBYTES(Number) };

	return true;
}

bool ULockstepParamInt64::FromBytes(const TArray<uint8>& Data)
{
	if (Data.Num() != 8)
		return false;

	Number = BYTESTOINT64(Data.GetData());

	return true;
}

bool ULockstepParamInt64::ToBytes(TArray<uint8>& Data)
{
	Data = { INT64TOBYTES(Number) };

	return true;
}
