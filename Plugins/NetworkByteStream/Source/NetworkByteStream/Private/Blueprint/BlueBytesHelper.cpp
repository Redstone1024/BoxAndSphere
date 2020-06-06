// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/BlueBytesHelper.h"

#include "BytesHelper.h"

TArray<uint8> UBytesHelper::Int32ToBytes(int32 Data)
{
	return TArray<uint8>({ INT32TOBYTES(Data) });
}

TArray<uint8> UBytesHelper::Int64ToBytes(int64 Data)
{
	return TArray<uint8>({ INT64TOBYTES(Data) });
}

int32 UBytesHelper::BytesToInt32(const TArray<uint8>& Data)
{
	return static_cast<int32>(BYTESTOINT32(Data.GetData()));
}

int64 UBytesHelper::BytesToInt64(const TArray<uint8>& Data)
{
	return static_cast<int64>(BYTESTOINT64(Data.GetData()));
}

FString UBytesHelper::CharToString(uint8 Data)
{
	FString Result = "T";
	Result[0] = Data;
	return Result;
}

uint8 UBytesHelper::StringToChar(const FString & Data)
{
	return static_cast<uint8>(Data[0]);
}

uint8 UBytesHelper::ComputeCheck(const TArray<uint8>& Data)
{
	return FBytesHelper::ComputeCheck(Data);
}
