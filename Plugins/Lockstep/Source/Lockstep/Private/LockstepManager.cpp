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

	ELockstepReturn Return = SetRoundPass(IP, Port, RoundName, Password);
	if (Return != ELockstepReturn::Success)
		return Return;

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

	ELockstepReturn Return = SetRoundPass(IP, Port, RoundName, Password);
	if (Return != ELockstepReturn::Success)
		return Return;

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

	switch (NetMode)
	{
	case ELockstepNetMode::TCP:
		ConnectMaker = TSharedPtr<FConnectClientMaker>(new FConnectClientMakerTCP());
		break;
	}

	TSharedPtr<FByteStream> TempStream;
	TempStream = ConnectMaker->Construct(Pass.IPStr, Pass.Port);
	if (TempStream == nullptr)
		return ELockstepReturn::ConnectError;

	TempStream->Send({ CMD, INT64TOBYTES(Pass.RoundNumber), INT64TOBYTES(Pass.Password) });

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
	EventNameToIndex.Empty();

	EventSignatures.Empty();
	EventDelegates.Empty();

	EventParamObjects.Empty();

	for (auto& SystemEvent : SystemEventRegistry)
		RegisterEvent(SystemEvent.Key, SystemEvent.Value);

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

	TempBuffer.SetNum(0, false);
	Stream->Recv(TempBuffer);

	if (TempBuffer.Num() > 0)
	{

		RecvMessage.Append(TempBuffer);

		while (RecvMessage.Num() - NextMessageIndex >= 13)
		{
			int32 ParamsLen = BYTESTOINT32(RecvMessage.GetData() + NextMessageIndex + 8);
			if (ParamsLen > (RecvMessage.Num() - NextMessageIndex - 1))
				break;

			NewReceivedEvent.ID = BYTESTOINT32(RecvMessage.GetData() + NextMessageIndex + 0);

			if (IsFirstEvent)
			{
				LastEventID = NewReceivedEvent.ID - 1;
				IsFirstEvent = false;
			}
			
			if (NewReceivedEvent.ID != LastEventID + 1)
			{
				UE_LOG(LogLockstep, Warning, TEXT("Time ID jump in '%s'"), *GetFName().ToString());
				OnThrowError.Broadcast(ELockstepError::EventIDError);
				Status = ELockstepStatus::Unknown;
				Stream = nullptr;
				break;
			}
			
			LastEventID = NewReceivedEvent.ID;

			NewReceivedEvent.CMD = BYTESTOINT32(RecvMessage.GetData() + NextMessageIndex + 4);

			NewReceivedEvent.Params.SetNum(0, false);
			NewReceivedEvent.Params.Append(RecvMessage.GetData() + NextMessageIndex + 12, ParamsLen);

			NewReceivedEvent.Check = RecvMessage[NextMessageIndex + 12 + ParamsLen];

			if (NewReceivedEvent.Check != FBytesHelper::ComputeCheck(RecvMessage, NextMessageIndex, NextMessageIndex + 12 + ParamsLen))
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event [%d] check error in '%s'"), NewReceivedEvent.ID, *GetFName().ToString());
				OnThrowError.Broadcast(ELockstepError::CheckError);
				Status = ELockstepStatus::Unknown;
				Stream = nullptr;
				break;
			}

			if (NewReceivedEvent.CMD < static_cast<uint32>(EventSignatures.Num()))
			{
				ULockstepParamBase* ParamObject = EventParamObjects[EventSignatures[NewReceivedEvent.CMD].ParamType];
				if (ParamObject->FromBytes(NewReceivedEvent.Params))
				{
					EventDelegates[NewReceivedEvent.CMD].Broadcast(ParamObject);
				}
				else
				{
					UE_LOG(LogLockstep, Warning, TEXT("Event parameter resolution failed in '%s'"), NewReceivedEvent.ID, *GetFName().ToString());
					OnThrowError.Broadcast(ELockstepError::ParamError);
				}
			}
			else
			{
				UE_LOG(LogLockstep, Warning, TEXT("Event [%d] cmd id [%d] unregistered in '%s'"), NewReceivedEvent.ID, NewReceivedEvent.CMD, *GetFName().ToString());
				OnThrowError.Broadcast(ELockstepError::CmdError);
			}

			NextMessageIndex += 12 + ParamsLen + 1;
		}

	}
	else
	{
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

	check(Stream != nullptr);

	int32 EventIndex;
	FLockstepEventSignature EventSignature;
	if (!GetEventInfo(Name, EventIndex, EventSignature))
	{
		UE_LOG(LogLockstep, Error, TEXT("Trying to send an event '%s' that doesn't exist in '%s'"), *Name.ToString(), *GetFName().ToString());
		return false;
	}

	if (Params->GetClass() != EventSignature.ParamType)
	{
		UE_LOG(LogLockstep, Error, TEXT("Event parameter types do not match in '%s'"), *GetFName().ToString());
		return false;
	}

	FEvent ReadyEvent;
	if (!Params->ToBytes(ReadyEvent.Params))
	{
		UE_LOG(LogLockstep, Error, TEXT("Event parameter generation failed in '%s'"), *GetFName().ToString());
		return false;
	}

	ReadyEvent.ID = 0;
	ReadyEvent.CMD = EventIndex;

	TempBuffer.SetNum(0, false);
	TempBuffer.Append({ INT32TOBYTES(ReadyEvent.ID), INT32TOBYTES(ReadyEvent.CMD), INT32TOBYTES(ReadyEvent.Params.Num()) });
	TempBuffer.Append(ReadyEvent.Params);

	ReadyEvent.Check = FBytesHelper::ComputeCheck(TempBuffer);
	TempBuffer.Append({ ReadyEvent.Check });
	SendMessage.Append(TempBuffer);
	
	Stream->Send(TempBuffer);

	return true;
}
