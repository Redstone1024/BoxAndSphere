// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Engine/Engine.h"
#include "Serialization/MemoryWriter.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Serialization/MemoryReader.h"
#include "Engine/UserDefinedStruct.h"
#include "LockstepSubsystem.generated.h"

// 发生的错误类型
UENUM(BlueprintType)
enum class ELockstepError : uint8
{
	Reset,        // 在正常情况下重置
	Timeout,      // 超时
	CheckError,   // 校验码错误
	CmdError,     // 命令不存在
	EventIDError, // 事件编号跳跃
	Unknown,      // 未知状态
};

// 描述当前的锁步管理器状态
UENUM(BlueprintType)
enum class ELockstepStatus : uint8
{
	NoConnection,  // 未连接
	Normal,        // 正常
	Timeout,       // 超时
	Unknown,       // 未知状态
};

// 锁步管理器函数的返回值
UENUM(BlueprintType)
enum class ELockstepReturn : uint8
{
	Success,        // 一切正常

	WrongStatus,    // 状态异常

	ParameterError, // 参数异常
	ConnectError,   // 连接异常
	TimeoutError,   // 超时异常

	Reregister,     // 回合重复注册
	UnknownRound,   // 未知回合
	WrongPassword,  // 密码验证失败

	Unknown,        // 未知
};

// 连接方式
UENUM(BlueprintType)
enum class ELockstepNetMode : uint8
{
	TCP,
};

USTRUCT(BlueprintInternalUseOnly)
struct LOCKSTEP_API FLockstepEvent { GENERATED_BODY() };

DECLARE_DELEGATE_TwoParams(FLockstepEventDelegate, int32, void*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FLockstepEventDelegates, int32, void*);

DECLARE_DELEGATE_TwoParams(FLockstepTickDelegate, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FLockstepTickDelegates, int32, int32);

USTRUCT(BlueprintInternalUseOnly)
struct LOCKSTEP_API FLockstepEventPtr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Lockstep")
	UScriptStruct* EventStruct;

	void* RawPtr;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FLockstepEventDynamicDelegate, int32, EventID, FLockstepEventPtr, Params);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLockstepEventDynamicDelegates, int32, EventID, FLockstepEventPtr, Params);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FLockstepTickDynamicDelegate, int32, EventID, int32, TickID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLockstepTickDynamicDelegates, int32, EventID, int32, TickID);

DECLARE_DELEGATE_OneParam(FLockstepErrorDelegate, ELockstepError);
DECLARE_MULTICAST_DELEGATE_OneParam(FLockstepErrorDelegates, ELockstepError);

DECLARE_DYNAMIC_DELEGATE_OneParam(FLockstepErrorDynamicDelegate, ELockstepError, FailureType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepErrorDynamicDelegates, ELockstepError, FailureType);

UCLASS(Config=Engine, DefaultConfig)
class LOCKSTEP_API ULockstepSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	ULockstepSubsystem(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Config, EditAnywhere, Category = "Lockstep")
	uint32 bAutoTickUpdate : 1;

	UPROPERTY(Config, EditAnywhere, Category = "Lockstep")
	uint32 bAutoSendEmptyEvent : 1;

	UPROPERTY(Config, EditAnywhere, Category = "Lockstep")
	uint32 bTreatUserDefinedStructAsEvent : 1;

	struct FSessionPass
	{
		FString IPStr;
		unsigned short Port;

		uint64 Number;
		uint64 Password;
	};

	ELockstepReturn RegisterSession(const FSessionPass& NewPass);
	ELockstepReturn JoinSession(const FSessionPass& NewPass);

	void Update();
	void Reset();

	void BindErrorDelegate(FLockstepErrorDelegate Delegate);
	void BindErrorDynamicDelegate(FLockstepErrorDynamicDelegate Delegate);

	void BindTickDelegate(FLockstepTickDelegate Delegate);
	bool BindEventDelegate(UScriptStruct* EventType, FLockstepEventDelegate Delegate);

	void BindTickDynamicDelegate(FLockstepTickDynamicDelegate Delegate);
	bool BindEventDynamicDelegate(UScriptStruct* EventType, FLockstepEventDynamicDelegate Delegate);

	bool SendLockstepEvent(UScriptStruct* EventType, void* EventParams);
	void PrintLogOnServer(const FString& Str);

	FTimespan GetTimeoutLimit() const { return TimeoutLimit; }
	FTimespan SetTimeoutLimit(FTimespan InTimeoutLimit) { return TimeoutLimit = InTimeoutLimit; }
	ELockstepNetMode GetNetMode() const { return NetMode; }
	ELockstepNetMode SetNetMode(ELockstepNetMode NewNetMode);
	ELockstepStatus GetStatus() const { return Status; }
	FSessionPass GetSessionPass() const { return Pass; }
	uint32 GetEventIndex(UScriptStruct* EventType) const;
	UScriptStruct* GetEventType(uint32 EventIndex) const;

private:
	void HandlingError(ELockstepError Error);
	FLockstepErrorDelegates ErrorDelegates;
	FLockstepErrorDynamicDelegates ErrorDynamicDelegates;

	struct FEvent
	{
		uint32 ID;            // 编号
		uint32 CMD;           // 命令
		TArray<uint8> Params; // 参数
		uint8 Check;          // 校验码
	};

	ELockstepReturn ConnectSession(const FSessionPass& NewPass, uint8 CMD);

	FSessionPass Pass;
	ELockstepStatus Status;
	ELockstepNetMode NetMode;
	FTimespan TimeoutLimit;

	TSharedPtr<class FByteStream> Stream;

	void InitializeStructTable();

	UPROPERTY() TArray<UScriptStruct*> NetStructTable;
	UPROPERTY() TMap<UScriptStruct*, uint32> NetIndexTable;

	FLockstepTickDelegates TickDelegates;
	TArray<FLockstepEventDelegates> NetDelegatesTable;

	UPROPERTY() FLockstepTickDynamicDelegates TickDynamicDelegates;
	UPROPERTY() TArray<FLockstepEventDynamicDelegates> NetDynamicDelegatesTable;

	void HandlingNetworkReceive();
	void HandlingSendEmptyEvent();
	void HandlingSystemEvent(const FEvent& Event);
	void HandlingUserEvent(const FEvent& Event);
	void HandlingEventSend(const FEvent& Event);
	FDateTime LastEventSendTime;
	bool bIsFirstEvent;
	int32 LastEventID;
	int32 NextPendingIndex = 0;
	TArray<uint8> SendMessage;
	TArray<uint8> RecvMessage;

	// 临时变量 在发送事件和接收事件时都会用到
	FEvent TempEvent;
	TArray<uint8> TempMessage;
	
public:
	//~ Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override { }
	//~ End USubsystem Interface

	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override { Update(); }
	virtual bool IsTickable() const override { return !IsTemplate() && bAutoTickUpdate; }
	virtual TStatId GetStatId() const override { return GetStatID(); }
	//~ End FTickableGameObject Interface
};
