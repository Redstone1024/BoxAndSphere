// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Templates/SharedPointer.h"

class FByteStream;

/**
 * �����ߣ�
 * ��Ϊ����˴�����[ConnectServerMaker]���Ϳͻ��˴�����[ConnectClientMaker]
 * �໥��Ӧ�����ڴ�����ͬ��Э��ջ��������������ֽ�����
 * �ͻ��˴����߹��������е�PactĬ��ֵӦ���������������ͬ
 * �����������ߵ�IDӦ���ڹ��������и�����ΨһID
 *
 * ע�⣺��PactΪ0��ʱ����Ҫ�����Լ�����ID��������ֱ�Ӵ�0��ȥ��
 */

class NETWORKBYTESTREAM_API FConnectClientMaker
{
public:
	virtual TSharedPtr<FByteStream> Construct(const FString& IP, unsigned short Port, uint8 Pact = 0 /* �˴������������ID��ͬ */) = 0;

	virtual ~FConnectClientMaker() { }
};

class NETWORKBYTESTREAM_API FConnectServerMaker
{
public:
	virtual uint8 GetPactID() { return 0 /* �˴���ͻ��˲���ID��ͬ */; }
	virtual TSharedPtr<FByteStream> Construct(TSharedPtr<class FSocket> Sock) = 0;

	virtual ~FConnectServerMaker() { }
};
