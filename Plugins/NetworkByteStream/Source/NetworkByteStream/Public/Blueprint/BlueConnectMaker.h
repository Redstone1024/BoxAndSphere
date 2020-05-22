// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlueConnectMaker.generated.h"

class UByteStream;

UENUM(BlueprintType)
enum class EConnectMaker : uint8
{
	TCP
};

UCLASS(BlueprintType)
class NETWORKBYTESTREAM_API UConnectMaker : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UByteStream* Construct(const FString& IP, int Port, EConnectMaker Pact);

};
