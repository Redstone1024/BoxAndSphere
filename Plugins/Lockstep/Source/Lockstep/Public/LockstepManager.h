// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockstepEventSignature.h"
#include "Delegates/DelegateCombinations.h"
#include "LockstepManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FLockstepEventDelegate, ULockstepParamBase*, Params);            // 事件回调
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepEventDelegates, ULockstepParamBase*, Params); // 事件多播回调

// 发送的错误类型
UENUM(BlueprintType)
enum class ELockstepError : uint8
{
	Timeout,      // 超时
	CheckError,   // 校验码错误
	CmdError,     // 命令不存在
	ParamError,   // 参数解析出错
	EventIDError, // 事件编号跳跃
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepErrorDelegate, ELockstepError, FailureType); // 错误回调

class UDataTable;

// 描述当前的锁步管理器状态
UENUM(BlueprintType)
enum class ELockstepStatus : uint8
{
	Uninitialized, // 未初始化
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

// 锁步系统的中央管理器
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepManager : public UObject
{
	GENERATED_BODY()

	// 状态控制
private:
	ELockstepStatus Status = ELockstepStatus::Uninitialized; // 当前的状态

public:
	ULockstepManager();

	// 初始化管理器
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void Initialization();

	// 更新一次管理器
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void Update();

	// 获取当前的状态
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepStatus GetStatus() const { return Status; }

	// 出现错误时的回调
	UPROPERTY(BlueprintAssignable, Category = "Lockstep")
	FLockstepErrorDelegate OnThrowError;

	// 连接控制
private:
	// 连接信息
	struct FRoundPass
	{
		FString IPStr;       // 地址
		unsigned short Port; // 端口

		uint64 RoundNumber;  // 房间名Hash值
		uint64 Password;     // 密码Hash值
	};

	FRoundPass Pass;

	TSharedPtr<class FByteStream> Stream; // 网络流

public:
	// 在服务器上注册一个回合
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepReturn RegisterRound(const FString& IP, int Port, const FString& RoundName, const FString& Password);

	// 加入服务器的一个回合
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepReturn JoinRound(const FString& IP, int Port, const FString& RoundName, const FString& Password);

	// 获取当前回合号
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lockstep")
	int64 GetRoundNumber() const { return static_cast<int64>(Pass.RoundNumber); }

private:
	ELockstepReturn SetRoundPass(const FString& IP, int Port, const FString& RoundName, const FString& Password);
	ELockstepReturn ConnectRound(uint8 CMD);

	// 基础设置
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	ELockstepNetMode NetMode = ELockstepNetMode::TCP; // 当前网络连接模式
	
	FTimespan TimeoutLimit = FTimespan::FromSeconds(5); // 超时限制

	// 事件注册
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	UDataTable* EventRegistry; // 事件注册表

private:
	const TArray<TPair<FName, FLockstepEventSignature>> SystemEventRegistry =
	{
		TPair<FName, FLockstepEventSignature>(TEXT("Void"), FLockstepEventSignature(ULockstepParamVoid::StaticClass())),  // [00] 空事件主要用于防止误判超时
		TPair<FName, FLockstepEventSignature>(TEXT("Tick"), FLockstepEventSignature(ULockstepParamInt32::StaticClass())), // [01] Tick事件 由服务器发起
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [02] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [03] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [04] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [05] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [06] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [07] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [08] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [09] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [10] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [11] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [12] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [13] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [14] 保留
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [15] 保留
	};

	TMap<FName, int32> EventNameToIndex; // 事件名称到索引的映射
	
	TArray<FLockstepEventSignature> EventSignatures; // 事件签名
	TArray<FLockstepEventDelegates> EventDelegates;  // 事件回调

	UPROPERTY()
	TMap<TSubclassOf<ULockstepParamBase>, ULockstepParamBase*> EventParamObjects; // 事件参数池

	void InitEventRegistry();
	void RegisterEvent(const FName& Name, const FLockstepEventSignature& Signature);

public:
	// 获取事件的信息
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool GetEventInfo(FName Name, int32& Index, FLockstepEventSignature& Signature) const;

	// 通过索引获取事件的信息
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool GetEventInfoByIndex(int32 Index, FLockstepEventSignature& Signature) const;

	// 绑定一个事件的回调
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void BindEvent(FName Name, FLockstepEventDelegate Event);

	// 通过索引绑定一个事件的回调
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void BindEventByIndex(int32 Index, FLockstepEventDelegate Event);

	// 事件处理
private:
	// 事件原始结构
	struct FEvent
	{
		uint32 ID;            // 编号
		uint32 CMD;           // 命令
		TArray<uint8> Params; // 参数
		uint8 Check;          // 校验码
	};

	bool IsFirstEvent = true;   // 是第一个事件吗
	int32 LastEventID;          // 最后一个受到的完整事件编号
	FEvent TempEvent;           // 事件缓冲
	int32 NextMessageIndex = 0; // 下一个待处理的字节索引
	TArray<uint8> SendMessage;  // 发送的消息
	TArray<uint8> RecvMessage;  // 接收的消息
	TArray<uint8> TempBuffer;   // 消息缓冲

	void HandlingNetworkReceive(); // 处理网络消息接收

public:
	// 发送一个网络事件
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool SendEvent(FName Name, ULockstepParamBase* Params);

	// 通过索引发送一个网络事件
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool SendEventByIndex(int32 Index, ULockstepParamBase* Params);

	// 获取最后一个受到的完整事件编号
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	int32 GetLastEventID() const { return LastEventID; }
};
