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
 * 监听者[ConnectListener]，存在于服务器，单独开一个线程监听连接请求，收到请求后
 * 根据需要的流类型[TCP/UDP等]调用与之对应的创建者。
 *
 * 网络字节流构建思路：
 * 服务器                                客户端
 * 监听者                                             创建者
 * |  开始监听，等待客户端的请求 <------ 发送连接请求      |
 * |  请求接收到后，查看流类型编号 <---- 发送流类型编号    |
 * |  根据流类型编号，调用对应创建者                       |
 * |创建者                                                 |
 * || 建立流                                               |
 * || 发送建立完成信息 ----------------> 收到建立成功信息  |
 * || 返回流指针                         返回流指针        |
 * |  将流指针压入队列等待主线程来取
 * |  等待更多客户端请求
 */

class NETWORKBYTESTREAM_API FConnectListener
{
public:
	FConnectListener(const FString& IP, unsigned short Port, const FAvailablePactsType& AvailablePacts);
	~FConnectListener();

	// 控制监听的开始和结束
	bool Start();
	void Stop();

	// 是否在监听
	bool IsListening() { return Listening; }

	// 获取最新的监听结果
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
