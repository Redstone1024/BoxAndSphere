// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockstepEventSignature.h"
#include "Delegates/DelegateCombinations.h"
#include "LockstepManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FLockstepEventDelegate, ULockstepParamBase*, Params);            // �¼��ص�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepEventDelegates, ULockstepParamBase*, Params); // �¼��ಥ�ص�

// ���͵Ĵ�������
UENUM(BlueprintType)
enum class ELockstepError : uint8
{
	Timeout,      // ��ʱ
	CheckError,   // У�������
	CmdError,     // �������
	ParamError,   // ������������
	EventIDError, // �¼������Ծ
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepErrorDelegate, ELockstepError, FailureType); // ����ص�

class UDataTable;

// ������ǰ������������״̬
UENUM(BlueprintType)
enum class ELockstepStatus : uint8
{
	Uninitialized, // δ��ʼ��
	NoConnection,  // δ����
	Normal,        // ����
	Timeout,       // ��ʱ
	Unknown,       // δ֪״̬
};

// ���������������ķ���ֵ
UENUM(BlueprintType)
enum class ELockstepReturn : uint8
{
	Success,        // һ������

	WrongStatus,    // ״̬�쳣

	ParameterError, // �����쳣
	ConnectError,   // �����쳣
	TimeoutError,   // ��ʱ�쳣
	
	Reregister,     // �غ��ظ�ע��
	UnknownRound,   // δ֪�غ�
	WrongPassword,  // ������֤ʧ��

	Unknown,        // δ֪
};

// ���ӷ�ʽ
UENUM(BlueprintType)
enum class ELockstepNetMode : uint8
{
	TCP,
};

// ����ϵͳ�����������
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepManager : public UObject
{
	GENERATED_BODY()

	// ״̬����
private:
	ELockstepStatus Status = ELockstepStatus::Uninitialized; // ��ǰ��״̬

public:
	ULockstepManager();

	// ��ʼ��������
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void Initialization();

	// ����һ�ι�����
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void Update();

	// ��ȡ��ǰ��״̬
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepStatus GetStatus() const { return Status; }

	// ���ִ���ʱ�Ļص�
	UPROPERTY(BlueprintAssignable, Category = "Lockstep")
	FLockstepErrorDelegate OnThrowError;

	// ���ӿ���
private:
	// ������Ϣ
	struct FRoundPass
	{
		FString IPStr;       // ��ַ
		unsigned short Port; // �˿�

		uint64 RoundNumber;  // ������Hashֵ
		uint64 Password;     // ����Hashֵ
	};

	FRoundPass Pass;

	TSharedPtr<class FByteStream> Stream; // ������

public:
	// �ڷ�������ע��һ���غ�
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepReturn RegisterRound(const FString& IP, int Port, const FString& RoundName, const FString& Password);

	// �����������һ���غ�
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	ELockstepReturn JoinRound(const FString& IP, int Port, const FString& RoundName, const FString& Password);

	// ��ȡ��ǰ�غϺ�
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lockstep")
	int64 GetRoundNumber() const { return static_cast<int64>(Pass.RoundNumber); }

private:
	ELockstepReturn SetRoundPass(const FString& IP, int Port, const FString& RoundName, const FString& Password);
	ELockstepReturn ConnectRound(uint8 CMD);

	// ��������
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	ELockstepNetMode NetMode = ELockstepNetMode::TCP; // ��ǰ��������ģʽ
	
	FTimespan TimeoutLimit = FTimespan::FromSeconds(5); // ��ʱ����

	// �¼�ע��
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	UDataTable* EventRegistry; // �¼�ע���

private:
	const TArray<TPair<FName, FLockstepEventSignature>> SystemEventRegistry =
	{
		TPair<FName, FLockstepEventSignature>(TEXT("Void"), FLockstepEventSignature(ULockstepParamVoid::StaticClass())),  // [00] ���¼���Ҫ���ڷ�ֹ���г�ʱ
		TPair<FName, FLockstepEventSignature>(TEXT("Tick"), FLockstepEventSignature(ULockstepParamInt32::StaticClass())), // [01] Tick�¼� �ɷ���������
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [02] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [03] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [04] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [05] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [06] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [07] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [08] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [09] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [10] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [11] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [12] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [13] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [14] ����
		TPair<FName, FLockstepEventSignature>(NAME_None, FLockstepEventSignature(ULockstepParamVoid::StaticClass())),     // [15] ����
	};

	TMap<FName, int32> EventNameToIndex; // �¼����Ƶ�������ӳ��
	
	TArray<FLockstepEventSignature> EventSignatures; // �¼�ǩ��
	TArray<FLockstepEventDelegates> EventDelegates;  // �¼��ص�

	UPROPERTY()
	TMap<TSubclassOf<ULockstepParamBase>, ULockstepParamBase*> EventParamObjects; // �¼�������

	void InitEventRegistry();
	void RegisterEvent(const FName& Name, const FLockstepEventSignature& Signature);

public:
	// ��ȡ�¼�����Ϣ
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool GetEventInfo(FName Name, int32& Index, FLockstepEventSignature& Signature) const;

	// ͨ��������ȡ�¼�����Ϣ
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool GetEventInfoByIndex(int32 Index, FLockstepEventSignature& Signature) const;

	// ��һ���¼��Ļص�
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void BindEvent(FName Name, FLockstepEventDelegate Event);

	// ͨ��������һ���¼��Ļص�
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	void BindEventByIndex(int32 Index, FLockstepEventDelegate Event);

	// �¼�����
private:
	// �¼�ԭʼ�ṹ
	struct FEvent
	{
		uint32 ID;            // ���
		uint32 CMD;           // ����
		TArray<uint8> Params; // ����
		uint8 Check;          // У����
	};

	bool IsFirstEvent = true;   // �ǵ�һ���¼���
	int32 LastEventID;          // ���һ���ܵ��������¼����
	FEvent TempEvent;           // �¼�����
	int32 NextMessageIndex = 0; // ��һ����������ֽ�����
	TArray<uint8> SendMessage;  // ���͵���Ϣ
	TArray<uint8> RecvMessage;  // ���յ���Ϣ
	TArray<uint8> TempBuffer;   // ��Ϣ����

	void HandlingNetworkReceive(); // ����������Ϣ����

public:
	// ����һ�������¼�
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool SendEvent(FName Name, ULockstepParamBase* Params);

	// ͨ����������һ�������¼�
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	bool SendEventByIndex(int32 Index, ULockstepParamBase* Params);

	// ��ȡ���һ���ܵ��������¼����
	UFUNCTION(BlueprintCallable, Category = "Lockstep")
	int32 GetLastEventID() const { return LastEventID; }
};
