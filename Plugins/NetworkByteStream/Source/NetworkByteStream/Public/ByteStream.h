// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * �����ֽ���[ByteStream]����Ϊ����Ự�Ĵ��룬ֻ�л������շ����ܡ�
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
