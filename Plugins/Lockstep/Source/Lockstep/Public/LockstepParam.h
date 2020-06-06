// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockstepParam.generated.h"

UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamBase : public UObject
{
	GENERATED_BODY()

public:

	virtual bool FromBytes(const TArray<uint8>& Data) { return true; }
	virtual bool ToBytes(TArray<uint8>& Data) { return true; }
};

UCLASS(BlueprintType, Blueprintable)
class LOCKSTEP_API ULockstepParamVoid : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	TArray<uint8> RawData;

	virtual bool FromBytes(const TArray<uint8>& Data) final { RawData = Data; return ParBytes(); }
	virtual bool ToBytes(TArray<uint8>& Data) final { if (GenBytes()) { Data = RawData; return true; } else { return false; } }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	bool ParBytes();
	bool ParBytes_Implementation() { return true; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockstep")
	bool GenBytes();
	bool GenBytes_Implementation() { return true; }
};

UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamInt32 : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	int32 Number;

	virtual bool FromBytes(const TArray<uint8>& Data) final;
	virtual bool ToBytes(TArray<uint8>& Data) final;
};

UCLASS(BlueprintType)
class LOCKSTEP_API ULockstepParamInt64 : public ULockstepParamBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	int64 Number;

	virtual bool FromBytes(const TArray<uint8>& Data) final;
	virtual bool ToBytes(TArray<uint8>& Data) final;
};
