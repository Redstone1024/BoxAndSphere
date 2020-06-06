// Fill out your copyright notice in the Description page of Project Settings.


#include "ByteStreamTCP.h"

#include "Sockets.h"

FByteStreamTCP::FByteStreamTCP(TSharedPtr<FSocket> pSock)
	: Sock(pSock)
	, LastActiveTime(FDateTime::Now())
{
	Sock->SetNoDelay();
}

void FByteStreamTCP::Send(const TArray<uint8>& Data)
{
	int32 BytesSent;
	Sock->Send(Data.GetData(), Data.Num(), BytesSent);
}

void FByteStreamTCP::Recv(TArray<uint8>& Data, uint32 MaxBytesRead)
{
	int32 BytesRead;
	uint32 PendingDataSize;
	if (Sock->HasPendingData(PendingDataSize))
	{
		PendingDataSize = FMath::Min(PendingDataSize, MaxBytesRead);
		Data.SetNum(PendingDataSize, false);
		Sock->Recv(Data.GetData(), PendingDataSize, BytesRead);
		LastActiveTime = FDateTime::Now();
	}
	else Data.Empty();
}

void FByteStreamTCP::Update() { }
