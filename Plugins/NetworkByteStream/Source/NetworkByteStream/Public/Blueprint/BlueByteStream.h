// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlueByteStream.generated.h"

UCLASS(BlueprintType)
class NETWORKBYTESTREAM_API UByteStream : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "NetworkByteStream")
	void Send(const TArray<uint8>& Data);
	
	UFUNCTION(BlueprintCallable, Category = "NetworkByteStream")
	void Recv(TArray<uint8>& Data);
	
	UFUNCTION(BlueprintCallable, Category = "NetworkByteStream")
	void Update();

	TSharedPtr<class FByteStream> ByteStreamPtr;

};
