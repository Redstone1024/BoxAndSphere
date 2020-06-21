// Fill out your copyright notice in the Description page of Project Settings.


#include "LockstepManager.h"

#include "Lockstep.h"

#include "ByteStream.h"
#include "BytesHelper.h"
#include "ConnectMaker.h"
#include "ConnectMaker/ConnectMakerTCP.h"

#include "Engine/Engine.h"

ULockstepManager::ULockstepManager()
{ }

void ULockstepManager::Initialization()
{
	if (Status != ELockstepStatus::Uninitialized)
		return;

	InitEventRegistry();

	Status = ELockstepStatus::NoConnection;

	UE_LOG(LogLockstep, Log, TEXT("Initialized '%s'"), *GetFName().ToString());
}

void ULockstepManager::Update()
{
	HandlingNetworkReceive();
}

ELockstepReturn ULockstepManager::RegisterRound(const FString & IP, int Port, const FString& RoundName, const FString& Password)
{
	if (Status != ELockstepStatus::NoConnection)
		return ELockstepReturn::WrongStatus;

	// 设置连接信息
	ELockstepReturn Return = SetRoundPass(IP, Port, RoundName, Password);
	if (Return != ELockstepReturn::Success)
		return Return;

	// 连接
	Return = ConnectRound('R');

	if (Return == ELockstepReturn::Success)
	{
		UE_LOG(LogLockstep, Log, TEXT("Register round success '%s'"), *GetFName().ToString());
	}
	else
	{
		UE_LOG(LogLockstep, Warning, TEXT("Register round fail '%s'"), *GetFName().ToString());
	}

	return Return;
}

ELockstepReturn ULockstepManager::JoinRound(const FString & IP, int Port, const FString& RoundName, const FString& Password)
{
	if (Status != ELockstepStatus::NoConnection)
		return ELockstepReturn::WrongStatus;

	// 设置连接信息
	ELockstepReturn Return = SetRoundPass(IP, Port, RoundName, Password);
	if (Return != ELockstepReturn::Success)
		return Return;

	// 连接
	Return = ConnectRound('L');

	if (Return == ELockstepReturn::Success)
	{
		UE_LOG(LogLockstep, Log, TEXT("Join round success '%s'"), *GetFName().ToString());
	}
	else
	{
		UE_LOG(LogLockstep, Warning, TEXT("Join round fail '%s'"), *GetFName().ToString());
	}
	return Return;
}

ELockstepReturn ULockstepManager::SetRoundPass(const FString & IP, int Port, const FString & RoundName, const FString & Password)
{
	// 确保参数范围正常
	if (IP.IsEmpty() || Port <= 0 || Port > UINT16_MAX || RoundName.IsEmpty() || Password.IsEmpty())
		return ELockstepReturn::ParameterError;

	Pass.IPStr = IP;
	Pass.Port = Port;
	Pass.RoundNumber = GetTypeHash(RoundName);
	Pass.Password = GetTypeHash(Password);

	return ELockstepReturn::Success;
}

ELockstepReturn ULockstepManager::ConnectRound(uint8 CMD)
{
	TSharedPtr<FConnectClientMaker> ConnectMaker;

	// 选择连接模式
	switch (NetMode)
	{
	case ELockstepNetMode::TCP:
		ConnectMaker = TSharedPtr<FConnectClientMaker>(new FConnectClientMakerTCP());
		break;
	}

	// 连接服务器
	TSharedPtr<FByteStream> TempStream;
	TempStream = ConnectMaker->Construct(Pass.IPStr, Pass.Port);
	if (TempStream == nullptr)
		return ELockstepReturn::ConnectError;

	// 发送房间请求
	TempStream->Send({ CMD, INT64TOBYTES(Pass.RoundNumber), INT64TOBYTES(Pass.Password) });

	// 接收返回值
	TArray<uint8> Return;
	do 
	{
		if (FDateTime::Now() - TempStream->GetLastActiveTime() > TimeoutLimit)
			return ELockstepReturn::TimeoutError;

		TempStream->Recv(Return, 1);
	}
	while (Return.Num() < 1);

	if (Return.Num() != 1)
		return ELockstepReturn::Unknown;

	// 处理返回值为枚举
	switch (Return[0])
	{
	case 'T':
		Stream = TempStream;
		Status = ELockstepStatus::Normal;
		return ELockstepReturn::Success;
	case 'F':
		return ELockstepReturn::Unknown;
	case 'K':
		return ELockstepReturn::Reregister;
	case 'I':
		return ELockstepReturn::UnknownRound;
	case 'P':
		return ELockstepReturn::WrongPassword;
	}

	return ELockstepReturn::Unknown;
}

void ULockstepManager::InitEventRegistry()
{
	// 保证数据为空
	EventNameToIndex.Empty();
	EventSignatures.Empty();
	EventDelegates.Empty();
	EventParamObjects.Empty();

	TickEventDelegates.Clear();

	// 注册系统事件
	RegisterSystemEvent();

	// 如果用户注册表存在则注册
	if (!IsValid(EventRegistry))
		return;

	TArray<FName> EventNames = EventRegistry->GetRowNames();

	FLockstepEventSignature* Signature = nullptr;
	for (FName& EventName : EventNames)
	{
		Signature = EventRegistry->FindRow<FLockstepEventSignature>(EventName, GetFName().ToString());
		if (Signature != nullptr)
			RegisterEvent(EventName, *Signature);
	}

}

void ULockstepManager::RegisterEvent(const FName & Name, const FLockstepEventSignature & Signature)
{
	if (EventNameToIndex.Contains(Name))
	{
		UE_LOG(LogLockstep, Error, TEXT("Register duplicate events to '%s'"), *GetFName().ToString());
		return;
	}

	int32 NewIndex = EventSignatures.Add(Signature);
	EventDelegates.SetNum(NewIndex + 1);

	// 当名称未指定时 事件只能通过索引使用
	if (Name != NAME_None)
		EventNameToIndex.Add(Name, NewIndex);
	
	if (!EventParamObjects.Contains(Signature.ParamType))
		EventParamObjects.Add(Signature.ParamType, NewObject<ULockstepParamBase>(this, Signature.ParamType));

}

bool ULockstepManager::GetEventInfo(FName Name, int32 & Index, FLockstepEventSignature & Signature) const
{
	if (!EventNameToIndex.Contains(Name))
	{
		UE_LOG(LogLockstep, Warning, TEXT("Trying to get information about a non-existent event '%s' in '%s'"), *Name.ToString(), *GetFName().ToString());
		Index = -1;
		Signature = FLockstepEventSignature();
		return false;
	}

	Index = EventNameToIndex[Name];
	Signature = EventSignatures[Index];

	return true;
}

bool ULockstepManager::GetEventInfoByIndex(int32 Index, FLockstepEventSignature & Signature) const
{
	if (Index < 0 || EventSignatures.Num() <= Index)
	{
		UE_LOG(LogLockstep, Warning, TEXT("Trying to get information about a non-existent event index [%d] in '%s'"), Index, *GetFName().ToString());
		return false;
	}

	Signature = EventSignatures[Index];

	return true;
}

void ULockstepManager::BindEvent(FName Name, FLockstepEventDelegate Event)
{
	if (!EventNameToIndex.Contains(Name))
	{
		UE_LOG(LogLockstep, Error, TEXT("Trying to bind the event to an unregistered delegate '%s' in '%s'"), *Name.ToString(), *GetFName().ToString());
		return;
	}

	EventDelegates[EventNameToIndex[Name]].Add(Event);
}

void ULockstepManager::BindEventByIndex(int32 Index, FLockstepEventDelegate Event)
{
	if (Index < 0 || EventDelegates.Num() <= Index)
	{
		UE_LOG(LogLockstep, Error, TEXT("Trying to bind the event to an unregistered delegate index [%d] in '%s'"), Index, *GetFName().ToString());
		return;
	}

	EventDelegates[Index].Add(Event);
}

void ULockstepManager::HandlingNetworkReceive()
{
	if (Status != ELockstepStatus::Normal)
		return;

	check(Stream != nullptr);

	// 接收新消息
	TempBuffer.SetNum(0, false);
	Stream->Recv(TempBuffer);

	if (TempBuffer.Num() > 0)
	{
		// 追加消息到处理缓冲
		RecvMessage.Append(TempBuffer);

		// 如果未处理的消息长度大于最小消息长度则处理
		while (RecvMessage.Num() - NextMessageIndex >= 13)
		{
			// 处理参数长度
			int32 ParamsLen = BYTESTOINT32(RecvMessage.GetData() + NextMessageIndex + 8);
			if (ParamsLen > (RecvMessage.Num() - NextMessageIndex - 1))
				break;

			// 处理参数编号
			TempEvent.ID = BYTESTOINT32(RecvMessage.GetData() + NextMessageIndex + 0);

			if (IsFirstEvent)
			{
				LastEventID = TempEvent.ID - 1;
				IsFirstEvent = false;
			}
			
			if (TempEvent.ID != LastEventID + 1)
			{
				UE_LOG(LogLockstep, Warning, TEXT("Time ID jump in '%s'"), *GetFName().ToString());
				OnThrowError.Broadcast(ELockstepError::EventIDError);
				Status = ELockstepStatus::Unknown;
				Stream = nullptr;
				break;
			}
			
			LastEventID = TempEvent.ID;

			// 读取指令 参数 校验码
			TempEvent.CMD = BYTESTOINT32(RecvMessage.GetData() + NextMessageIndex + 4);
			TempEvent.Params.SetNum(0, false);
			TempEvent.Params.Append(RecvMessage.GetData() + NextMessageIndex + 12, ParamsLen);
			TempEvent.Check = RecvMessage[NextMessageIndex + 12 + ParamsLen];

			// 验证校验码
			if (TempEvent.Check != FBytesHelper::ComputeCheck(RecvMessage, NextMessageIndex, NextMessageIndex + 12 + ParamsLen))
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event [%d] check error in '%s'"), TempEvent.ID, *GetFName().ToString());
				OnThrowError.Broadcast(ELockstepError::CheckError);
				Status = ELockstepStatus::Unknown;
				Stream = nullptr;
				break;
			}

			// 验证消息被注册
			if (TempEvent.CMD < static_cast<uint32>(EventSignatures.Num()))
			{
				// 验证参数是否可以被解析
				ULockstepParamBase* ParamObject = EventParamObjects[EventSignatures[TempEvent.CMD].ParamType];
				if (ParamObject->FromBytes(TempEvent.Params))
				{
					// 系统事件特殊处理
					if (TempEvent.CMD < 16)
						HandlingSystemEvent(TempEvent, ParamObject);
					else
						EventDelegates[TempEvent.CMD].Broadcast(TempEvent.ID, ParamObject);
				}
				else
				{
					UE_LOG(LogLockstep, Warning, TEXT("Event parameter resolution failed in '%s'"), TempEvent.ID, *GetFName().ToString());
					OnThrowError.Broadcast(ELockstepError::ParamError);
				}
			}
			else
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event [%d] cmd id [%d] unregistered in '%s'"), TempEvent.ID, TempEvent.CMD, *GetFName().ToString());
				OnThrowError.Broadcast(ELockstepError::CmdError);
			}

			// 将待处理标记后移
			NextMessageIndex += 12 + ParamsLen + 1;
		}

	}
	else
	{
		// 检测是否超时
		if (FDateTime::Now() - Stream->GetLastActiveTime() > TimeoutLimit)
		{
			UE_LOG(LogLockstep, Warning, TEXT("Connection timed out in '%s'"), *GetFName().ToString());
			OnThrowError.Broadcast(ELockstepError::Timeout);
			Status = ELockstepStatus::Timeout;
			Stream = nullptr;
			return;
		}
	}

}

bool ULockstepManager::SendEvent(FName Name, ULockstepParamBase * Params)
{
	if (Status != ELockstepStatus::Normal)
		return false;

	// 尝试获取事件索引
	int32 EventIndex;
	FLockstepEventSignature EventSignature;
	if (!GetEventInfo(Name, EventIndex, EventSignature))
	{
		UE_LOG(LogLockstep, Error, TEXT("Trying to send an event '%s' that doesn't exist in '%s'"), *Name.ToString(), *GetFName().ToString());
		return false;
	}

	// 获取索引成功则发送
	return SendEventByIndex(EventIndex, Params);
}

bool ULockstepManager::SendEventByIndex(int32 Index, ULockstepParamBase * Params)
{
	if (Status != ELockstepStatus::Normal)
		return false;

	check(Stream != nullptr);

	// 尝试获取事件签名
	FLockstepEventSignature EventSignature;
	if (!GetEventInfoByIndex(Index, EventSignature))
	{
		UE_LOG(LogLockstep, Error, TEXT("Try to send a non-existent event [%d] in '%s'"), Index, *GetFName().ToString());
		return false;
	}

	// 如果是空指针则使用默认参数
	if (!IsValid(Params))
	{
		Params = EventParamObjects[EventSignature.ParamType];
		Params->Reset();
	}

	// 检测参数类型是否正确
	if (Params->GetClass() != EventSignature.ParamType)
	{
		UE_LOG(LogLockstep, Error, TEXT("Event parameter types do not match in '%s'"), *GetFName().ToString());
		return false;
	}

	// 检测参数解析是否正确
	if (!Params->ToBytes(TempEvent.Params))
	{
		UE_LOG(LogLockstep, Error, TEXT("Event parameter generation failed in '%s'"), *GetFName().ToString());
		return false;
	}

	// 填充事件
	TempEvent.ID = 0;
	TempEvent.CMD = Index;

	TempBuffer.SetNum(0, false);
	TempBuffer.Append({ INT32TOBYTES(TempEvent.ID), INT32TOBYTES(TempEvent.CMD), INT32TOBYTES(TempEvent.Params.Num()) });
	TempBuffer.Append(TempEvent.Params);

	TempEvent.Check = FBytesHelper::ComputeCheck(TempBuffer);
	TempBuffer.Append({ TempEvent.Check });
	SendMessage.Append(TempBuffer);

	// 发送事件
	Stream->Send(TempBuffer);

	return true;
}

void ULockstepManager::RegisterSystemEvent()
{
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [00] 空事件
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamInt32::StaticClass())); // [01] Tick事件
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [02]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [03]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [04]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [05]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [06]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [07]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [08]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [09]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [10]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [11]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [12]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [13]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [14]
	RegisterEvent(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass()));  // [15]
}

void ULockstepManager::HandlingSystemEvent(const FEvent & Event, ULockstepParamBase* Param)
{
	switch (Event.CMD)
	{
	case 0:
		break;
	case 1:
	{
		check(Cast<ULockstepParamInt32>(Param));
		ULockstepParamInt32* ParamInt32 = (ULockstepParamInt32*)Param;
		TickEventDelegates.Broadcast(Event.ID, ParamInt32->Value);
		break;
	}
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		UE_LOG(LogLockstep, Warning, TEXT("Unknown system event in '%s'"), *GetFName().ToString());
		break;
	default:
		checkNoEntry();
		break;
	}
}
