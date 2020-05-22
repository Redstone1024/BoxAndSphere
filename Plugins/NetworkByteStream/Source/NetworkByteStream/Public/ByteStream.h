// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 网络字节流[ByteStream]，作为网络会话的代码，只有基础的收发功能。
 */

class NETWORKBYTESTREAM_API FByteStream
{
protected:
	FByteStream() { };

public:
	virtual ~FByteStream() { }

	virtual void Send(const TArray<uint8>& Data) = 0;
	virtual void Recv(TArray<uint8>& Data) = 0;
	virtual void Update() { }

	FByteStream(const FByteStream&) = delete;
	FByteStream(const FByteStream&&) = delete;
	FByteStream& operator=(const FByteStream&) = delete;
	FByteStream& operator=(const FByteStream&&) = delete;
};
