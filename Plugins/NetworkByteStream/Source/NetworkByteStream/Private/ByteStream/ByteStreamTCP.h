// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ByteStream.h"
#include "Templates/SharedPointer.h"

class FSocket;

class FByteStreamTCP : public FByteStream
{
	friend class FConnectClientMakerTCP;
	friend class FConnectServerMakerTCP;

private:
	TSharedPtr<FSocket> Sock;

	FByteStreamTCP(TSharedPtr<FSocket> pSock) : Sock(pSock) { };

public:
	virtual ~FByteStreamTCP() final { }

	virtual void Send(const TArray<uint8>& Data) final;
	virtual void Recv(TArray<uint8>& Data) final;
	virtual void Update() final;
};
