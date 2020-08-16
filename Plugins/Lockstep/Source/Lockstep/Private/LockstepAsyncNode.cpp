// Fill out your copyright notice in the Description page of Project Settings.


#include "LockstepAsyncNode.h"

#include "Kismet/GameplayStatics.h"
#include "Interfaces/IPv4/IPv4Address.h"

namespace
{
	DECLARE_DELEGATE_OneParam(FSessionTaskDelegate, ELockstepReturn);
	
	// 会话操作完成时的任务 用于通知游戏线程
	class FSessionTaskCompleted
	{
	public:
		FSessionTaskCompleted(FSessionTaskDelegate InTaskDelegate, ELockstepReturn InResult) :
			TaskDelegate(InTaskDelegate), Result(InResult)
		{ }

		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			check(IsInGameThread());
			TaskDelegate.ExecuteIfBound(Result);
		}

		FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(FSessionTaskCompletion, STATGROUP_TaskGraphTasks); }
		static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::GameThread; } // 注意这里是游戏线程
		static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	private:
		FSessionTaskDelegate TaskDelegate;
		ELockstepReturn Result;
	};
	
	FAutoConsoleTaskPriority CPrio_Session(
		TEXT("TaskGraph.TaskPriorities.Session"),
		TEXT("Task and thread priority for lockstep session."),
		ENamedThreads::HighThreadPriority,
		ENamedThreads::NormalTaskPriority,
		ENamedThreads::HighTaskPriority
	);

	// 会话任务
	class FSessionTask
	{
	public:
		FSessionTask(ULockstepSubsystem* InLockstepSubsystem, ULockstepSubsystem::FSessionPass InSessionPass, FSessionTaskDelegate InTaskDelegate)
			: LockstepSubsystem(InLockstepSubsystem), SessionPass(InSessionPass), TaskDelegate(InTaskDelegate)
		{ }

		FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(FRegisterSessionTask, STATGROUP_TaskGraphTasks); }
		static ENamedThreads::Type GetDesiredThread() { return CPrio_Session.Get(); } // 选择最优线程
		static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	protected:
		ULockstepSubsystem* LockstepSubsystem;
		ULockstepSubsystem::FSessionPass SessionPass;

		FSessionTaskDelegate TaskDelegate;
	};

	// 注册会话任务
	class FRegisterSessionTask : public FSessionTask
	{
	public:
		FRegisterSessionTask(ULockstepSubsystem* InLockstepSubsystem, ULockstepSubsystem::FSessionPass InSessionPass, FSessionTaskDelegate InTaskDelegate)
			: FSessionTask(InLockstepSubsystem, InSessionPass, InTaskDelegate)
		{ }

		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			// 调用会话注册函数
			ELockstepReturn Result = LockstepSubsystem->RegisterSession(SessionPass);

			// 任务完成后通知游戏线程
			FGraphEventRef ChildTask = TGraphTask<FSessionTaskCompleted>::CreateTask(nullptr, CurrentThread).
				ConstructAndDispatchWhenReady(TaskDelegate, Result);
			MyCompletionGraphEvent->SetGatherThreadForDontCompleteUntil(ENamedThreads::GameThread);
			MyCompletionGraphEvent->DontCompleteUntil(ChildTask);
		}
	};

	// 加入会话任务
	class FJoinSessionTask : public FSessionTask
	{
	public:
		FJoinSessionTask(ULockstepSubsystem* InLockstepSubsystem, ULockstepSubsystem::FSessionPass InSessionPass, FSessionTaskDelegate InTaskDelegate)
			: FSessionTask(InLockstepSubsystem, InSessionPass, InTaskDelegate)
		{ }

		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			// 调用会话注册函数
			ELockstepReturn Result = LockstepSubsystem->JoinSession(SessionPass);

			// 任务完成后通知游戏线程
			FGraphEventRef ChildTask = TGraphTask<FSessionTaskCompleted>::CreateTask(nullptr, CurrentThread).
				ConstructAndDispatchWhenReady(TaskDelegate, Result);
			MyCompletionGraphEvent->SetGatherThreadForDontCompleteUntil(ENamedThreads::GameThread);
			MyCompletionGraphEvent->DontCompleteUntil(ChildTask);
		}
	};

	// 尝试根据数据生成会话通行证
	bool GenerateSessionPass(ULockstepSubsystem::FSessionPass& Result, const FString& IPStr, const FString& PortStr, const FString& NumberStr, const FString& PasswordStr, bool bUseHash)
	{
		FIPv4Address IP;
		bool bIsValidIP = FIPv4Address::Parse(IPStr, IP);

		if (!bIsValidIP)
			return false;

		Result.IPStr = IPStr;

		Result.Port = FCString::Atoi(*PortStr);

		if (Result.Port < 0 || Result.Port > UINT16_MAX)
			return false;

		if (bUseHash)
		{
			Result.Number = GetTypeHash(NumberStr);
			Result.Password = GetTypeHash(PasswordStr);
		}
		else
		{
			LexFromString(Result.Number, *NumberStr);
			LexFromString(Result.Password, *PasswordStr);

			if (!Result.Number || !Result.Password)
				return false;
		}

		return true;
	}
}

void ULockstepSessionAction::Completed(ELockstepReturn Result)
{
	if (Result == ELockstepReturn::Success) OnSuccess.Broadcast(Result);
	else OnFailure.Broadcast(Result);
}

URegisterLockstepSessionAction * URegisterLockstepSessionAction::RegisterLockstepSession(UObject * WorldContextObject, const FString & IP, const FString & Port, const FString & Number, const FString & Password, bool bUseHash)
{
	URegisterLockstepSessionAction* Task = NewObject<URegisterLockstepSessionAction>();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	Task->LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	Task->bSessionPassValid = GenerateSessionPass(Task->SessionPass, IP, Port, Number, Password, bUseHash);

	return Task;
}

void URegisterLockstepSessionAction::Activate()
{
	FSessionTaskDelegate CompletedDelegate;
	CompletedDelegate.BindUFunction(this, TEXT("Completed"));

	if (bSessionPassValid)
		TGraphTask<FRegisterSessionTask>::CreateTask(nullptr, ENamedThreads::GameThread)
		.ConstructAndDispatchWhenReady(LockstepSubsystem, SessionPass, CompletedDelegate);
	else Completed(ELockstepReturn::ParameterError);
}

UJoinLockstepSessionAction * UJoinLockstepSessionAction::JoinLockstepSession(UObject * WorldContextObject, const FString & IP, const FString & Port, const FString & Number, const FString & Password, bool bUseHash)
{
	UJoinLockstepSessionAction* Task = NewObject<UJoinLockstepSessionAction>();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	Task->LockstepSubsystem = GameInstance->GetSubsystem<ULockstepSubsystem>();
	Task->bSessionPassValid = GenerateSessionPass(Task->SessionPass, IP, Port, Number, Password, bUseHash);

	return Task;
}

void UJoinLockstepSessionAction::Activate()
{
	FSessionTaskDelegate CompletedDelegate;
	CompletedDelegate.BindUFunction(this, TEXT("Completed"));

	if (bSessionPassValid)
		TGraphTask<FJoinSessionTask>::CreateTask(nullptr, ENamedThreads::GameThread)
		.ConstructAndDispatchWhenReady(LockstepSubsystem, SessionPass, CompletedDelegate);
	else Completed(ELockstepReturn::ParameterError);
}
