// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/BlueConnectMaker.h"

#include "ConnectMaker/ConnectMakerTCP.h"
#include "Blueprint/BlueByteStream.h"

class FConnectClientMaker;

UByteStream* UConnectMaker::Construct(const FString& IP, int Port, EConnectMaker Pact)
{
	TSharedPtr<FConnectClientMaker> Maker;

	switch (Pact)
	{
	case EConnectMaker::TCP:
		Maker = TSharedPtr<FConnectClientMaker>(new FConnectClientMakerTCP());
		break;
	}

	TSharedPtr<class FByteStream> ByteStreamPtr = Maker->Construct(IP, static_cast<unsigned short>(Port));
	if (!ByteStreamPtr) return nullptr;
	
	UByteStream* Result = NewObject<UByteStream>();
	Result->ByteStreamPtr = ByteStreamPtr;
	return Result;
}
