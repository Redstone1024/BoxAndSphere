// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Templates/SharedPointer.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"
#include "HAL/Runnable.h"

class FSocket;
class FByteStream;
class FConnectServerMaker;

typedef TArray<TSharedPtr<FConnectServerMaker>> FAvailablePactsType;
typedef TMap<uint8, TSharedPtr<FConnectServerMaker>> FAvailablePactsMap;

/**
 * ������[ConnectListener]�������ڷ�������������һ���̼߳������������յ������
 * ������Ҫ��������[TCP/UDP��]������֮��Ӧ�Ĵ����ߡ�
 *
 * �����ֽ�������˼·��
 * ������                                �ͻ���
 * ������                                             ������
 * |  ��ʼ�������ȴ��ͻ��˵����� <------ ������������      |
 * |  ������յ��󣬲鿴�����ͱ�� <---- ���������ͱ��    |
 * |  ���������ͱ�ţ����ö�Ӧ������                       |
 * |������                                                 |
 * || ������                                               |
 * || ���ͽ��������Ϣ ----------------> �յ������ɹ���Ϣ  |
 * || ������ָ��                         ������ָ��        |
 * |  ����ָ��ѹ����еȴ����߳���ȡ
 * |  �ȴ�����ͻ�������
 */

class NETWORKBYTESTREAM_API FConnectListener
{
public:
	FConnectListener(const FString& IP, unsigned short Port, const FAvailablePactsType& AvailablePacts);
	~FConnectListener();

	// ���Ƽ����Ŀ�ʼ�ͽ���
	bool Start();
	void Stop();

	// �Ƿ��ڼ���
	bool IsListening() { return Listening; }

	// ��ȡ���µļ������
	TSharedPtr<FByteStream> TryGetConnection();

private:
	class FListenRunnable : public FRunnable
	{
	private:
		bool Stopping = true;
		FConnectListener* Listener;
		TSharedPtr<FSocket> Socket;

	public:
		FListenRunnable(FConnectListener* pListener, TSharedPtr<FSocket> ListenSocket)
			: Listener(pListener)
			, Socket(ListenSocket)
		{ }

		virtual bool Init() final;
		virtual uint32 Run() final;
		virtual void Stop() final;
		virtual void Exit() final;
	};

	friend FListenRunnable;

private:

	FString IP;
	unsigned short Port;
	FAvailablePactsMap AvailablePacts;

	bool Listening;
	TSharedPtr<FRunnableThread> ListenThread;

	TQueue<TSharedPtr<FByteStream>> ConnectionQueue;
};
