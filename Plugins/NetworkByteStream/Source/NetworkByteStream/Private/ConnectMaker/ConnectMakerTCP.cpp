// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectMaker/ConnectMakerTCP.h"

#include "ByteStream/ByteStreamTCP.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Sockets.h"

TSharedPtr<FByteStream> FConnectClientMakerTCP::Construct(const FString & IP, unsigned short Port, uint8 Pact)
{
	TSharedPtr<FByteStreamTCP> Result = nullptr;

	if (Pact == 0)
		Pact = 1;

	uint8 Data;
	int32 BytesNum;

	FIPv4Address IPv4;
	FIPv4Address::Parse(IP, IPv4);

	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IPv4.Value);
	Addr->SetPort(Port);

	TSharedPtr<FSocket> Sock = TSharedPtr<FSocket>(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false));

	if (!Sock->Connect(*Addr)) return Result;
	if (!Sock->Send(&Pact, 1, BytesNum) || BytesNum != 1) return Result;

	if (!Sock->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1))) return Result;
	if (!Sock->Recv(&Data, 1, BytesNum) || BytesNum != 1 || Data != 'F') return Result;

	Result = TSharedPtr<FByteStreamTCP>(new FByteStreamTCP(Sock));

	return Result;
}

TSharedPtr<FByteStream> FConnectServerMakerTCP::Construct(TSharedPtr<FSocket> Sock)
{
	TSharedPtr<FByteStreamTCP> Result = nullptr;

	uint8 Data = 'F';
	int32 BytesNum;
	if (!Sock->Send(&Data, 1, BytesNum) || BytesNum != 1) return Result;

	Result = TSharedPtr<FByteStreamTCP>(new FByteStreamTCP(Sock));

	return Result;
}
