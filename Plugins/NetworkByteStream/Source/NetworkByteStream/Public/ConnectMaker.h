// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Templates/SharedPointer.h"

class FByteStream;

/**
 * 创建者：
 * 分为服务端创建者[ConnectServerMaker]，和客户端创建者[ConnectClientMaker]
 * 相互对应，用于创建不同的协议栈，最后抽象成网络字节流。
 * 客户端创建者构建函数中的Pact默认值应与服务器创建者相同
 * 服务器构建者的ID应该在构建函数中给予与唯一ID
 *
 * 注意：当Pact为0是时，需要子类自己处理ID，并不是直接传0过去。
 */

class NETWORKBYTESTREAM_API FConnectClientMaker
{
public:
	virtual TSharedPtr<FByteStream> Construct(const FString& IP, unsigned short Port, uint8 Pact = 0 /* 此处与服务器部分ID相同 */) = 0;

	virtual ~FConnectClientMaker() { }
};

class NETWORKBYTESTREAM_API FConnectServerMaker
{
public:
	virtual uint8 GetPactID() { return 0 /* 此处与客户端部分ID相同 */; }
	virtual TSharedPtr<FByteStream> Construct(TSharedPtr<class FSocket> Sock) = 0;

	virtual ~FConnectServerMaker() { }
};
