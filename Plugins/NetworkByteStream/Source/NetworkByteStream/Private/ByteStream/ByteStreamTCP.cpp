// Fill out your copyright notice in the Description page of Project Settings.


#include "ByteStreamTCP.h"

#include "Sockets.h"

void FByteStreamTCP::Send(const TArray<uint8>& Data)
{
	int32_t BytesSent;
	Sock->Send(Data.GetData(), Data.Num(), BytesSent);
}

void FByteStreamTCP::Recv(TArray<uint8>& Data)
{
	int32_t BytesRead;
	uint32_t PendingDataSize;
	if (Sock->HasPendingData(PendingDataSize))
	{
		Data.SetNum(PendingDataSize);
		Sock->Recv(Data.GetData(), PendingDataSize, BytesRead);
	}
	else Data.Empty();
}

void FByteStreamTCP::Update() { }
