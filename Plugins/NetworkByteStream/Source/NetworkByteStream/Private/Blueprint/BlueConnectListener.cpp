// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/BlueConnectListener.h"

#include "ConnectListener.h"

#include "ConnectMaker/ConnectMakerTCP.h"
#include "Blueprint/BlueByteStream.h"

UConnectListener* UConnectListener::MakeConnectListener(const FString& IP, int Port, EConnectListener Pact)
{
	switch (Pact)
	{
	case EConnectListener::TCP:

		UConnectListener* Result = NewObject<UConnectListener>();
		Result->ListenerPtr = TSharedPtr<FConnectListener>(new FConnectListener(IP, static_cast<unsigned short>(Port), { TSharedPtr<FConnectServerMaker>(new FConnectServerMakerTCP()) }));
		return Result;

	}

	return nullptr;
}

bool UConnectListener::Start()
{
	if (!ListenerPtr) return false;
	return ListenerPtr->Start();
}

void UConnectListener::Stop()
{
	if (!ListenerPtr) return;
	ListenerPtr->Stop();
}

bool UConnectListener::IsListening()
{
	if (!ListenerPtr) return false;
	return ListenerPtr->IsListening();
}

UByteStream * UConnectListener::TryGetConnection()
{
	if (!ListenerPtr) return nullptr;

	TSharedPtr<class FByteStream> ByteStreamPtr = ListenerPtr->TryGetConnection();
	if (!ByteStreamPtr) return nullptr;

	UByteStream* Result = NewObject<UByteStream>();
	Result->ByteStreamPtr = ByteStreamPtr;
	return Result;
}
