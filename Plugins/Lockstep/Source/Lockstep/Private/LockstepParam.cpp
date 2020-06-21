// Fill out your copyright notice in the Description page of Project Settings.


#include "LockstepParam.h"

#include "BytesHelper.h"

bool ULockstepParamBool::FromBytes(const TArray<uint8>& Data)
{
	if (Data.Num() != 1)
		return false;

	Value = Data[0];

	return true;
}

bool ULockstepParamBool::ToBytes(TArray<uint8>& Data)
{
	Data = { static_cast<uint8>(Value) };

	return true;
}

bool ULockstepParamInt32::FromBytes(const TArray<uint8>& Data)
{
	if (Data.Num() != 4)
		return false;

	Value = BYTESTOINT32(Data.GetData());

	return true;
}

bool ULockstepParamInt32::ToBytes(TArray<uint8>& Data)
{
	Data = { INT32TOBYTES(Value) };

	return true;
}

bool ULockstepParamInt64::FromBytes(const TArray<uint8>& Data)
{
	if (Data.Num() != 8)
		return false;

	Value = BYTESTOINT64(Data.GetData());

	return true;
}

bool ULockstepParamInt64::ToBytes(TArray<uint8>& Data)
{
	Data = { INT64TOBYTES(Value) };

	return true;
}

bool ULockstepParamString::FromBytes(const TArray<uint8>& Data)
{
	if (Data.Num() % 2)
		return false;

	int Length = Data.Num() / 2;
	Value.Empty();

	for (int i = 0; i < Length; i++)
		Value += (TCHAR)(BYTESTOINT16(Data.GetData() + (2 * i)));

	return true;
}

bool ULockstepParamString::ToBytes(TArray<uint8>& Data)
{
	Data.SetNum(Value.Len() * 2, false);

	for (int i = 0; i < Value.Len(); i++)
	{
		Data[2 * i + 0] = Value[i] >> 0;
		Data[2 * i + 1] = Value[i] >> 8;
	}

	return true;
}
