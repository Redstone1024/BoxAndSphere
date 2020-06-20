// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueConnectMaker.generated.h"

class UByteStream;

UENUM(BlueprintType)
enum class EConnectMaker : uint8
{
	TCP
};

UCLASS(BlueprintType)
class NETWORKBYTESTREAM_API UConnectMaker : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "NetworkByteStream")
	static UByteStream* ConnectToListener(const FString& IP, int Port, EConnectMaker Pact);

};
