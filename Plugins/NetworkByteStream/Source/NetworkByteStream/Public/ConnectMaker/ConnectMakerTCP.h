// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../ConnectMaker.h"
#include "Templates/SharedPointer.h"

class NETWORKBYTESTREAM_API FConnectClientMakerTCP : public FConnectClientMaker
{
public:
	virtual TSharedPtr<FByteStream> Construct(const FString& IP, unsigned short Port, uint8 Pact = 1) final;
};

class NETWORKBYTESTREAM_API FConnectServerMakerTCP : public FConnectServerMaker
{
public:
	virtual uint8 GetPactID() final { return 1; }
	virtual TSharedPtr<FByteStream> Construct(TSharedPtr<class FSocket> Sock) final;
};
