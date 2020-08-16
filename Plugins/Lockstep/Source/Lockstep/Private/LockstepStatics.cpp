// Fill out your copyright notice in the Description page of Project Settings.


#include "LockstepStatics.h"

#include "Lockstep.h"
#include "Kismet/GameplayStatics.h"

ELockstepNetMode ULockstepStatics::GetLockstepNetMode(const UObject * WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	return LockstepSubsystem->GetNetMode();
}

bool ULockstepStatics::SetLockstepNetMode(const UObject * WorldContextObject, ELockstepNetMode NewMode)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	return LockstepSubsystem->SetNetMode(NewMode) == NewMode;
}

ELockstepStatus ULockstepStatics::GetLockstepStatus(const UObject * WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	return LockstepSubsystem->GetStatus();
}

void ULockstepStatics::GetLockstepSessionPass(const UObject * WorldContextObject, FString & IP, FString & Port, FString & Number, FString & Password)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	ULockstepSubsystem::FSessionPass SessionPass = LockstepSubsystem->GetSessionPass();
	IP = SessionPass.IPStr;
	Port = FString::FromInt(SessionPass.Port);
	Number = FString::Printf(TEXT("%llu"), SessionPass.Number);
	Password = FString::Printf(TEXT("%llu"), SessionPass.Password);
}

void ULockstepStatics::PrintLogOnLockstepServer(const UObject * WorldContextObject, const FString & InString)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	if (LockstepSubsystem->GetStatus() == ELockstepStatus::Normal)
		LockstepSubsystem->PrintLogOnServer(InString);
}

void ULockstepStatics::Generic_SendLockstepEvent(const UObject * WorldContextObject, UScriptStruct * EventType, void * EventParams, const FString & StackTraceStr)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();

	if (EventType == nullptr)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Wrong type in SendLockstepEvent.\n%s"), *StackTraceStr), ELogVerbosity::Warning);
		return;
	}

	if (LockstepSubsystem->GetEventIndex(EventType) == UINT32_ERROR)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Event '%s' unregistered in SendLockstepEvent.\n%s"), *EventType->GetName(), *StackTraceStr), ELogVerbosity::Warning);
		return;
	}

	LockstepSubsystem->SendLockstepEvent(EventType, EventParams);
}

void ULockstepStatics::BindTickDelegate(const UObject * WorldContextObject, FLockstepTickDynamicDelegate Event)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	LockstepSubsystem->BindTickDynamicDelegate(Event);
}

void ULockstepStatics::BindEventDelegate(const UObject * WorldContextObject, UScriptStruct * EventType, FLockstepEventDynamicDelegate Event)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();

	if (EventType == nullptr)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Wrong type in BindEventDelegate.")), ELogVerbosity::Warning);
		return;
	}

	if (LockstepSubsystem->GetEventIndex(EventType) == UINT32_ERROR)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Event '%s' unregistered in BindEventDelegate."), *EventType->GetName()), ELogVerbosity::Warning);
		return;
	}

	LockstepSubsystem->BindEventDynamicDelegate(EventType, Event);
}

void ULockstepStatics::BindErrorDelegate(const UObject * WorldContextObject, FLockstepErrorDynamicDelegate Event)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	LockstepSubsystem->BindErrorDynamicDelegate(Event);
}

void ULockstepStatics::Generic_DereferenceEventPtr(const FLockstepEventPtr& EventPtr, UScriptStruct * EventType, void * Event, bool& bSuccess)
{
	if (EventType == nullptr)
	{
		bSuccess = false;
		return;
	}

	if (EventPtr.EventStruct != EventType)
	{
		bSuccess = false;
		return;
	}

	EventType->CopyScriptStruct(Event, EventPtr.RawPtr);

	bSuccess = true;
}

void ULockstepStatics::ResetLockstepSubsystem(const UObject * WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	ULockstepSubsystem* LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	LockstepSubsystem->Reset();
}
