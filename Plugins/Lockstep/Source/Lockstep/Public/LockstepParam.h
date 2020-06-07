// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockstepParam.generated.h"

// 锁步事件的参数基类
UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamBase : public UObject
{
	GENERATED_BODY()

	friend class ULockstepManager;

protected:
	virtual bool FromBytes(const TArray<uint8>& Data) { return true; } // 从字节解析出参数
	virtual bool ToBytes(TArray<uint8>& Data) { return true; }         // 将参数解析成字节
};

// 表示一个靠反射函数解析的锁步参数 用于支持蓝图
UCLASS(BlueprintType, Blueprintable)
class LOCKSTEP_API ULockstepParamVoid : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	TArray<uint8> RawData;

protected:
	virtual bool FromBytes(const TArray<uint8>& Data) final { RawData = Data; return ParBytes(); }
	virtual bool ToBytes(TArray<uint8>& Data) final { if (GenBytes()) { Data = RawData; return true; } else { return false; } }

	// 由蓝图实现 用于从字节解析出参数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	bool ParBytes();
	bool ParBytes_Implementation() { return true; }

	// 由蓝图实现 用于将参数解析成字节
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	bool GenBytes();
	bool GenBytes_Implementation() { return true; }
};

// 一个布尔锁步参数
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

// 一个32位整形锁步参数
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

// 一个64位整形锁步参数
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
