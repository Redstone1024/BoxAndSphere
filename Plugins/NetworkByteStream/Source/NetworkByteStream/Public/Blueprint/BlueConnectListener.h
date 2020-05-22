// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlueConnectListener.generated.h"

class UByteStream;

UENUM(BlueprintType)
enum class EConnectListener : uint8
{
	TCP
};

UCLASS(BlueprintType)
class NETWORKBYTESTREAM_API UConnectListener : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UConnectListener* MakeConnectListener(const FString& IP, int Port, EConnectListener Pact);

	UFUNCTION(BlueprintCallable)
	bool Start();

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsListening();

	UFUNCTION(BlueprintCallable)
	UByteStream* TryGetConnection();

	TSharedPtr<class FConnectListener> ListenerPtr;

};
