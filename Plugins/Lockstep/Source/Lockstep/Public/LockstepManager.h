// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockstepEventSignature.h"
#include "Delegates/DelegateCombinations.h"
#include "LockstepManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FLockstepEventDelegate, ULockstepParamBase*, Params);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepEventDelegates, ULockstepParamBase*, Params);

UENUM(BlueprintType)
enum class ELockstepError : uint8
{
	Timeout,
	CheckError,
	CmdError,
	ParamError,
	EventIDError,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepErrorDelegate, ELockstepError, FailureType);

class UDataTable;

UENUM(BlueprintType)
enum class ELockstepStatus : uint8
{
	Uninitialized,
	NoConnection,
	Normal,
	Timeout,
	Unknown,
};

UENUM(BlueprintType)
enum class ELockstepReturn : uint8
{
	Success,

	WrongStatus,

	ParameterError,
	ConnectError,
	TimeoutError,
	
	Reregister,
	UnknownRound,
	WrongPassword,

	Unknown,
};

UENUM(BlueprintType)
enum class ELockstepNetMode : uint8
{
	TCP,
};

UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepManager : public UObject
{
	GENERATED_BODY()

	// 状态控制
private:
	ELockstepStatus Status = ELockstepStatus::Uninitialized;

public:
	ULockstepManager();

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void Initialization();

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void Update();

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepStatus GetStatus() const { return Status; }

	UPROPERTY(BlueprintAssignable, Category = "Lockstep")
	FLockstepErrorDelegate OnThrowError;

	// 连接控制
private:
	struct FRoundPass
	{
		FString IPStr;
		unsigned short Port;

		uint64 RoundNumber;
		uint64 Password;
	};

	FRoundPass Pass;

	TSharedPtr<class FByteStream> Stream;

public:
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepReturn RegisterRound(const FString& IP, int Port, const FString& RoundName, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepReturn JoinRound(const FString& IP, int Port, const FString& RoundName, const FString& Password);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lockstep")
	int64 GetRoundNumber() const { return static_cast<int64>(Pass.RoundNumber); }

private:
	ELockstepReturn SetRoundPass(const FString& IP, int Port, const FString& RoundName, const FString& Password);
	ELockstepReturn ConnectRound(uint8 CMD);

	// 基础设置
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	ELockstepNetMode NetMode = ELockstepNetMode::TCP;

	FTimespan TimeoutLimit = FTimespan::FromSeconds(5);

	// 事件注册
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	UDataTable* EventRegistry;

private:
	const TArray<TPair<FName, FLockstepEventSignature>> SystemEventRegistry =
	{
		TPair<FName, FLockstepEventSignature>(TEXT("Void"), FLockstepEventSignature(ULockstepParamVoid::StaticClass())),  // 00
		TPair<FName, FLockstepEventSignature>(TEXT("Tick"), FLockstepEventSignature(ULockstepParamInt32::StaticClass())), // 01
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 02
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 03
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 04
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 05
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 06
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 07
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 08
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 09
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 10
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 11
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 12
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 13
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 14
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // 15
	};

	TMap<FName, int32> EventNameToIndex;

	TArray<FLockstepEventSignature> EventSignatures;
	TArray<FLockstepEventDelegates> EventDelegates;

	UPROPERTY()
	TMap<TSubclassOf<ULockstepParamBase>, ULockstepParamBase*> EventParamObjects;

	void InitEventRegistry();
	void RegisterEvent(const FName& Name, const FLockstepEventSignature& Signature);

public:
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool GetEventInfo(FName Name, int32& Index, FLockstepEventSignature& Signature) const;

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool GetEventInfoByIndex(int32 Index, FLockstepEventSignature& Signature) const;

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void BindEvent(FName Name, FLockstepEventDelegate Event);

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void BindEventByIndex(int32 Index, FLockstepEventDelegate Event);

	// 事件处理
private:
	struct FEvent
	{
		uint32 ID;
		uint32 CMD;
		TArray<uint8> Params;
		uint8 Check;
	};

	bool IsFirstEvent = true;
	int32 LastEventID;
	FEvent NewReceivedEvent;
	int32 NextMessageIndex = 0;
	TArray<uint8> SendMessage;
	TArray<uint8> RecvMessage;
	TArray<uint8> TempBuffer;

	void HandlingNetworkReceive();

public:
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool SendEvent(FName Name, ULockstepParamBase* Params);

	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	int32 GetLastEventID() const { return LastEventID; }
};
