// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/BlueByteStream.h"

#include "ByteStream.h"

void UByteStream::Send(const TArray<uint8>& Data)
{
	if (!ByteStreamPtr) return;
	ByteStreamPtr->Send(Data);
}

void UByteStream::Recv(TArray<uint8>& Data)
{
	if (!ByteStreamPtr) return;
	ByteStreamPtr->Recv(Data);
}

void UByteStream::Update()
{
	if (!ByteStreamPtr) return;
	ByteStreamPtr->Update();
}
