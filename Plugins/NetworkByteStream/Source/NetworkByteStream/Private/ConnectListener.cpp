// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectListener.h"

#include "NetworkByteStream.h"

#include "ConnectMaker.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "HAL/RunnableThread.h"

FConnectListener::FConnectListener(const FString & IP, unsigned short pPort, const FAvailablePactsType & pAvailablePacts)
	: IPStr(IP)
	, Port(pPort)
	, Listening(false)
{
	UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Build Complete"));

	for (auto& x : pAvailablePacts)
		AvailablePacts.Add(x->GetPactID(), x);
}

FConnectListener::~FConnectListener()
{
	Stop();
}

bool FConnectListener::Start()
{
	if (ListenThread != nullptr && Listening) return true;
	if (ListenThread != nullptr) Stop();

	FIPv4Address IP;
	FIPv4Address::Parse(IPStr, IP);
	
	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	TSharedPtr<FSocket> ListenSocket = TSharedPtr<FSocket>(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false));

	if (!ListenSocket->Bind(*Addr)) return false;

	UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Bind Success [%s:%d]"), *IPStr, Port);
	ListenThread = TSharedPtr<FRunnableThread>(FRunnableThread::Create(new FListenRunnable(this, ListenSocket), TEXT("ListenRunnable")));
	return true;
}

void FConnectListener::Stop()
{
	if (ListenThread)
	{
		ListenThread->Kill(true);
		ListenThread = nullptr;
	}
}

TSharedPtr<FByteStream> FConnectListener::TryGetConnection()
{
	TSharedPtr<FByteStream> Result;
	if (ConnectionQueue.Dequeue(Result))
		return Result;
	return TSharedPtr<FByteStream>(nullptr);
}

bool FConnectListener::FListenRunnable::Init()
{
	Stopping = false;
	return true;
}

uint32 FConnectListener::FListenRunnable::Run()
{
	Listener->Listening = true;
	UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Start"));

	TSharedPtr<FInternetAddr> ClientAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TSharedPtr<FSocket> ClientSock;
	TSharedPtr<FByteStream> ClientByteStream;

	uint8 TypeNumber;
	int32 BytesRead;

	while (!Stopping)
	{
		if (!Socket->Listen(0))
		{
			UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Listen Error"));
			break;
		}

		ClientSock = TSharedPtr<FSocket>(Socket->Accept(TEXT("")));

		if (ClientSock != nullptr && !Stopping)
		{
			ClientSock->GetPeerAddress(*ClientAddr);
			UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Accept [%s]"), *ClientAddr->ToString(true));

			if (!ClientSock->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1)))
			{
				UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Recv Timeout [%s]"), *ClientAddr->ToString(true));
				ClientSock = nullptr;
				continue;
			}

			if (!ClientSock->Recv(&TypeNumber, 1, BytesRead) || BytesRead != 1)
			{
				UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Recv Error [%s]"), *ClientAddr->ToString(true));
				ClientSock = nullptr;
				continue;
			}

			if (!Listener->AvailablePacts.Contains(TypeNumber))
			{
				UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener Unknown Pact [%s]"), *ClientAddr->ToString(true));
				ClientSock = nullptr;
				continue;
			}

			ClientByteStream = Listener->AvailablePacts[TypeNumber]->Construct(ClientSock);
			if (ClientByteStream)
			{
				UE_LOG(LogNetworkByteStream, Log, TEXT("Byte Stream [%s] Construct Success In ConnectListener"), *ClientAddr->ToString(true));
				Listener->ConnectionQueue.Enqueue(ClientByteStream);
			}
			else
				UE_LOG(LogNetworkByteStream, Log, TEXT("Byte Stream [%s] Construct Fail In ConnectListener"), *ClientAddr->ToString(true));

		}

		ClientByteStream = nullptr;
		ClientSock = nullptr;
	}

	UE_LOG(LogNetworkByteStream, Log, TEXT("ConnectListener End"));
	Listener->Listening = false;
	return 0;
}

void FConnectListener::FListenRunnable::Stop()
{
	Socket->Shutdown(ESocketShutdownMode::ReadWrite);
	Socket->Close();
	Stopping = true;
}

void FConnectListener::FListenRunnable::Exit()
{
	Socket->Shutdown(ESocketShutdownMode::ReadWrite);
	Socket->Close();
}
