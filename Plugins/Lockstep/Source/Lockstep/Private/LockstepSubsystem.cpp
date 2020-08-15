// Fill out your copyright notice in the Description page of Project Settings.


#include "LockstepSubsystem.h"

#include "Lockstep.h"
#include "ByteStream.h"
#include "BytesHelper.h"
#include "Engine/ObjectLibrary.h"
#include "UObject/UObjectIterator.h"
#include "ConnectMaker/ConnectMakerTCP.h"

ULockstepSubsystem::ULockstepSubsystem(const FObjectInitializer & ObjectInitializer)
	: bAutoTickUpdate(true)
	, bAutoSendEmptyEvent(true)
	, bTreatUserDefinedStructAsEvent(false)
	, Status(ELockstepStatus::NoConnection)
	, NetMode(ELockstepNetMode::TCP)
	, TimeoutLimit(FTimespan::FromSeconds(5))
	, LastEventSendTime(FDateTime::Now())
	, bIsFirstEvent(true)
{
	Pass.IPStr = TEXT("");
	Pass.Port = 0;
	Pass.Number = 0;
	Pass.Password = 0;
}

ELockstepNetMode ULockstepSubsystem::SetNetMode(ELockstepNetMode NewNetMode)
{
	if (Status == ELockstepStatus::NoConnection)
		NetMode = NewNetMode;
	return NetMode;
}

ELockstepReturn ULockstepSubsystem::RegisterSession(const FSessionPass & NewPass)
{
	return ConnectSession(NewPass, 'R');
}

ELockstepReturn ULockstepSubsystem::JoinSession(const FSessionPass & NewPass)
{
	return ConnectSession(NewPass, 'L');
}

void ULockstepSubsystem::Update()
{
	if (Status == ELockstepStatus::Normal)
		HandlingNetworkReceive();

	if (Status == ELockstepStatus::Normal && bAutoSendEmptyEvent)
		HandlingSendEmptyEvent();
}

void ULockstepSubsystem::Reset()
{
	if (Status == ELockstepStatus::Normal)
		HandlingError(ELockstepError::Reset);

	Pass.IPStr = TEXT("");
	Pass.Port = 0;
	Pass.Number = 0;
	Pass.Password = 0;

	bIsFirstEvent = true;

	Status = ELockstepStatus::NoConnection;
}

void ULockstepSubsystem::BindErrorDelegate(FLockstepErrorDelegate Delegate)
{
	ErrorDelegates.Add(Delegate);
}

void ULockstepSubsystem::BindErrorDynamicDelegate(FLockstepErrorDynamicDelegate Delegate)
{
	ErrorDynamicDelegates.Add(Delegate);
}

void ULockstepSubsystem::BindTickDelegate(FLockstepTickDelegate Delegate)
{
	TickDelegates.Add(Delegate);
}

bool ULockstepSubsystem::BindEventDelegate(UScriptStruct * EventType, FLockstepEventDelegate Delegate)
{
	check(EventType != nullptr);

	const uint32 Index = GetEventIndex(EventType);

	check(Index != UINT32_ERROR);

	NetDelegatesTable[Index].Add(Delegate);
	return true;
}

void ULockstepSubsystem::BindTickDynamicDelegate(FLockstepTickDynamicDelegate Delegate)
{
	TickDynamicDelegates.Add(Delegate);
}

bool ULockstepSubsystem::BindEventDynamicDelegate(UScriptStruct * EventType, FLockstepEventDynamicDelegate Delegate)
{
	check(EventType != nullptr);

	const uint32 Index = GetEventIndex(EventType);

	check(Index != UINT32_ERROR);

	NetDynamicDelegatesTable[Index].Add(Delegate);
	return true;
}

bool ULockstepSubsystem::SendLockstepEvent(UScriptStruct * EventType, void * EventParams)
{
	check(EventType != nullptr && EventParams != nullptr);

	const uint32 Index = GetEventIndex(EventType);

	check(Index != UINT32_ERROR);

	TempEvent.ID = 0;
	TempEvent.CMD = Index;

	FMemoryWriter Writer(TempEvent.Params);
	EventType->SerializeItem(Writer, EventParams, nullptr);

	HandlingEventSend(TempEvent);

	return true;
}

void ULockstepSubsystem::PrintLogOnServer(const FString& Str)
{
	check(Status == ELockstepStatus::Normal);

	TempEvent.ID = 0;
	TempEvent.CMD = 2;
	TempEvent.Params.SetNum(Str.Len(), false);

	for (int i = 0; i < Str.Len(); i++)
		TempEvent.Params[i] = Str[i] < UINT8_MAX ? Str[i] : '*';

	HandlingEventSend(TempEvent);
}

uint32 ULockstepSubsystem::GetEventIndex(UScriptStruct * EventType) const
{
	check(EventType != nullptr);

	const uint32* IndexPtr = NetIndexTable.Find(EventType);
	return IndexPtr ? *IndexPtr : UINT32_ERROR;
}

UScriptStruct * ULockstepSubsystem::GetEventType(uint32 EventIndex) const
{
	return NetStructTable.IsValidIndex(EventIndex) ? NetStructTable[EventIndex] : nullptr;
}

void ULockstepSubsystem::HandlingError(ELockstepError Error)
{
	UE_LOG(LogLockstep, Warning, TEXT("An Error Occurred in '%s'."), *GetName());

	switch (Error)
	{
	case ELockstepError::Reset:
		Status = ELockstepStatus::Unknown;
		break;
	case ELockstepError::Timeout:
		Status = ELockstepStatus::Timeout;
		break;
	case ELockstepError::EventIDError:
		Status = ELockstepStatus::Unknown;
		break;
	case ELockstepError::CmdError:
		Status = ELockstepStatus::Unknown;
		break;
	case ELockstepError::CheckError:
		Status = ELockstepStatus::Unknown;
		break;
	case ELockstepError::Unknown:
		Status = ELockstepStatus::Unknown;
		break;
	default:
		checkNoEntry();
		break;
	}

	Stream = nullptr;

	ErrorDelegates.Broadcast(Error);
	ErrorDynamicDelegates.Broadcast(Error);
}

ELockstepReturn ULockstepSubsystem::ConnectSession(const FSessionPass & NewPass, uint8 CMD)
{
	if (Status != ELockstepStatus::NoConnection)
		return ELockstepReturn::WrongStatus;

	Pass = NewPass;

	TSharedPtr<FConnectClientMaker> ConnectMaker;
	
	// 选择连接模式
	switch (NetMode)
	{
	case ELockstepNetMode::TCP:
		ConnectMaker = TSharedPtr<FConnectClientMaker>(new FConnectClientMakerTCP());
		break;
	default:
		checkNoEntry();
		break;
	}
	
	// 连接服务器
	TSharedPtr<FByteStream> TempStream;
	TempStream = ConnectMaker->Construct(Pass.IPStr, Pass.Port);
	if (TempStream == nullptr)
		return ELockstepReturn::ConnectError;

	// 发送房间请求
	TempStream->Send({ CMD, INT64TOBYTES(Pass.Number), INT64TOBYTES(Pass.Password) });

	// 接收返回值
	TArray<uint8> Return;
	do
	{
		if (FDateTime::Now() - TempStream->GetLastActiveTime() > TimeoutLimit)
			return ELockstepReturn::TimeoutError;

		TempStream->Recv(Return, 1);
	} while (Return.Num() < 1);

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

void ULockstepSubsystem::InitializeStructTable()
{
	TArray<UScriptStruct*> EventStructs;
	UScriptStruct* LockstepEventStruct = FLockstepEvent::StaticStruct();
	if (LockstepEventStruct)
	{
		for (TObjectIterator<UScriptStruct> It; It; ++It)
		{
			UScriptStruct* Struct = *It;
			const bool bBasedOnLockstepEventBase = Struct->IsChildOf(LockstepEventStruct) && (Struct != LockstepEventStruct);
			const bool bValidStruct = (Struct->GetOutermost() != GetTransientPackage());
			if (bBasedOnLockstepEventBase && bValidStruct)
				EventStructs.Add(Struct);
		}
	}

	if (bTreatUserDefinedStructAsEvent)
	{
		UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UUserDefinedStruct::StaticClass(), false, GIsEditor);

		ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game"));
		TArray<FAssetData> AssetDatas;
		ObjectLibrary->GetAssetDataList(AssetDatas);

		for (const FAssetData& AssetData : AssetDatas)
		{
			UScriptStruct* Struct = Cast<UScriptStruct>(AssetData.GetAsset());
			check(Struct);
			EventStructs.Add(Struct);
		}
	}

	EventStructs.Sort(
		[] (const UScriptStruct& A, const UScriptStruct& B) 
		{ 
			return A.GetName() != B.GetName() 
				? A.GetName() < B.GetName() 
				: A.GetPathName() < B.GetPathName(); 
		});

	NetStructTable.Init(nullptr, 16);
	
	for (UScriptStruct* Struct : EventStructs)
	{
		int32 Index = NetStructTable.Add(Struct);
		NetIndexTable.Add(Struct, Index);
		UE_LOG(LogLockstep, Log, TEXT("Event '%s' registered in '%s'."), *Struct->GetName(), *GetName());
	}

	NetDelegatesTable.SetNum(NetStructTable.Num());
	NetDynamicDelegatesTable.SetNum(NetStructTable.Num());
}

void ULockstepSubsystem::HandlingNetworkReceive()
{
	check(Stream);
	check(Status == ELockstepStatus::Normal);
	
	// 接收新消息
	TempMessage.SetNum(0, false);
	Stream->Recv(TempMessage);

	if (TempMessage.Num() > 0)
	{
		// 追加消息到处理缓冲
		RecvMessage.Append(TempMessage);

		// 如果未处理的消息长度大于最小消息长度则处理
		while (RecvMessage.Num() - NextPendingIndex >= 13)
		{
			// 处理参数长度
			int32 ParamsLen = BYTESTOINT32(RecvMessage.GetData() + NextPendingIndex + 8);
			if (ParamsLen > (RecvMessage.Num() - NextPendingIndex - 1))
				break;

			// 处理参数编号
			TempEvent.ID = BYTESTOINT32(RecvMessage.GetData() + NextPendingIndex + 0);

			if (bIsFirstEvent)
			{
				LastEventID = TempEvent.ID - 1;
				bIsFirstEvent = false;
			}

			if (TempEvent.ID != LastEventID + 1)
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event ID jump in '%s'."), *GetFName().ToString());
				HandlingError(ELockstepError::EventIDError);
				break;
			}

			LastEventID = TempEvent.ID;

			// 读取指令 参数 校验码
			TempEvent.CMD = BYTESTOINT32(RecvMessage.GetData() + NextPendingIndex + 4);
			TempEvent.Params.SetNum(0, false);
			TempEvent.Params.Append(RecvMessage.GetData() + NextPendingIndex + 12, ParamsLen);
			TempEvent.Check = RecvMessage[NextPendingIndex + 12 + ParamsLen];

			// 验证校验码
			if (TempEvent.Check != FBytesHelper::ComputeCheck(RecvMessage, NextPendingIndex, NextPendingIndex + 12 + ParamsLen))
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event [%d] check error in '%s'."), TempEvent.ID, *GetFName().ToString());
				HandlingError(ELockstepError::CheckError);
				break;
			}

			// 验证消息被注册
			if (NetStructTable.IsValidIndex(TempEvent.CMD))
			{
				// 验证参数是否可以被解析
				UScriptStruct* EventStruct = NetStructTable[TempEvent.CMD];
				if (EventStruct == nullptr)
					HandlingSystemEvent(TempEvent);
				else
					HandlingUserEvent(TempEvent);
			}
			else
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event [%d] cmd id [%d] unregistered in '%s'."), TempEvent.ID, TempEvent.CMD, *GetFName().ToString());
				HandlingError(ELockstepError::CmdError);
			}

			// 将待处理标记后移
			NextPendingIndex += 12 + ParamsLen + 1;
		}

	}
	else
	{
		// 检测是否超时
		if (FDateTime::Now() - Stream->GetLastActiveTime() > TimeoutLimit)
		{
			UE_LOG(LogLockstep, Warning, TEXT("Connection timed out in '%s'."), *GetFName().ToString());
			HandlingError(ELockstepError::Timeout);
			return;
		}
	}

}

void ULockstepSubsystem::HandlingSendEmptyEvent()
{ 
	check(Status == ELockstepStatus::Normal);

	if (FDateTime::Now() - LastEventSendTime > (TimeoutLimit / 2))
	{
		static const FEvent EmptyEvent = { 0, 0, { }, 0 };
		HandlingEventSend(EmptyEvent);
	}
}

void ULockstepSubsystem::HandlingSystemEvent(const FEvent & Event)
{
	switch (Event.CMD)
	{
	case 0: // 空事件忽略
		break;
	case 1: // Tick事件
	{
		int32 TickID = BYTESTOINT32(Event.Params.GetData());
		TickDelegates.Broadcast(Event.ID, TickID);
		TickDynamicDelegates.Broadcast(Event.ID, TickID);
		break;
	}
	case 2: // 申请一个服务器Log 不应该由服务器发送
	{
		UE_LOG(LogLockstep, Warning, TEXT("Server log from the server in '%s'."), *GetFName().ToString());
		break;
	}
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
		UE_LOG(LogLockstep, Warning, TEXT("Unknown system event in '%s'."), *GetFName().ToString());
		break;
	default:
		checkNoEntry();
		break;
	}
}

void ULockstepSubsystem::HandlingUserEvent(const FEvent & Event)
{
	UScriptStruct* EventStruct = NetStructTable[Event.CMD];
	FLockstepEventDelegates& EventDelegates = NetDelegatesTable[Event.CMD];
	FLockstepEventDynamicDelegates& EventDynamicDelegates = NetDynamicDelegatesTable[Event.CMD];

	void* EventParams = FMemory::Malloc(EventStruct->GetStructureSize());
	EventStruct->InitializeStruct(EventParams);
	FMemoryReader Reader(Event.Params);
	EventStruct->SerializeItem(Reader, EventParams, nullptr);

	EventDelegates.Broadcast(Event.ID, EventParams);
	EventDynamicDelegates.Broadcast(Event.ID, { EventStruct, EventParams });

	FMemory::Free(EventParams);
}

void ULockstepSubsystem::HandlingEventSend(const FEvent & Event)
{
	check(Status == ELockstepStatus::Normal && Stream != nullptr);

	check(Event.ID == 0);

	TempMessage.SetNum(0, false);
	TempMessage.Append({ INT32TOBYTES(Event.ID), INT32TOBYTES(Event.CMD), INT32TOBYTES(Event.Params.Num()) });
	TempMessage.Append(Event.Params);

	TempMessage.Append({ FBytesHelper::ComputeCheck(TempMessage) });
	SendMessage.Append(TempMessage);

	Stream->Send(TempMessage);
}

void ULockstepSubsystem::Initialize(FSubsystemCollectionBase & Collection)
{
	InitializeStructTable();

	UE_LOG(LogLockstep, Log, TEXT("Initialized '%s'."), *GetFName().ToString());
}
