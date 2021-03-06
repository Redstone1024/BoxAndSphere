// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueBytesHelper.generated.h"

UCLASS()
class NETWORKBYTESTREAM_API UBytesHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static TArray<uint8> Int32ToBytes(int32 Data);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static TArray<uint8> Int64ToBytes(int64 Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static int32 BytesToInt32(const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static int64 BytesToInt64(const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static FString CharToString(uint8 Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static uint8 StringToChar(const FString& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BytesHelper")
	static uint8 ComputeCheck(const TArray<uint8>& Data);
};
