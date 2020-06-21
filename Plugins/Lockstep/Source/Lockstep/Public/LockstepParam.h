// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockstepParam.generated.h"

// �����¼��Ĳ�������
UCLASS(Abstract, BlueprintType)
class LOCKSTEP_API ULockstepParamBase : public UObject
{
	GENERATED_BODY()

	friend class ULockstepManager;

protected:
	virtual void Reset() { } // ����ΪĬ�ϲ���

	virtual bool FromBytes(const TArray<uint8>& Data) PURE_VIRTUAL(ULockstepParamBase::FromBytes, return true;); // ���ֽڽ���������
	virtual bool ToBytes(TArray<uint8>& Data) PURE_VIRTUAL(ULockstepParamBase::ToBytes, return true;);           // �������������ֽ�
};

// ��ʾһ�������亯���������������� ����֧����ͼ
UCLASS(BlueprintType, Blueprintable)
class LOCKSTEP_API ULockstepParamVoid : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	TArray<uint8> RawData;

protected:
	virtual void Reset() final { ResetParam(); }

	virtual bool FromBytes(const TArray<uint8>& Data) final { RawData = Data; return ParBytes(); }
	virtual bool ToBytes(TArray<uint8>& Data) final { if (GenBytes()) { Data = RawData; return true; } else { return false; } }

	// ����ͼʵ�� ��������ΪĬ�ϲ���
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	void ResetParam();
	void ResetParam_Implementation() { RawData.Reset(); }

	// ����ͼʵ�� ���ڴ��ֽڽ���������
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	bool ParBytes();
	bool ParBytes_Implementation() { return true; }

	// ����ͼʵ�� ���ڽ������������ֽ�
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	bool GenBytes();
	bool GenBytes_Implementation() { return true; }
};

// һ��������������
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamBool : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	bool Value;

protected:
	virtual bool FromBytes(const TArray<uint8>& Data) final;
	virtual bool ToBytes(TArray<uint8>& Data) final;
};

// һ��32λ������������
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamInt32 : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	int32 Value;

protected:
	virtual bool FromBytes(const TArray<uint8>& Data) final;
	virtual bool ToBytes(TArray<uint8>& Data) final;
};

// һ��64λ������������
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamInt64 : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	int64 Value;

protected:
	virtual bool FromBytes(const TArray<uint8>& Data) final;
	virtual bool ToBytes(TArray<uint8>& Data) final;
};

// һ���ַ�����������
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamString : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	FString Value;

protected:
	virtual void Reset() final { Value.Reset(); }

	virtual bool FromBytes(const TArray<uint8>& Data) final;
	virtual bool ToBytes(TArray<uint8>& Data) final;
};
