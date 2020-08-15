// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockstepSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LockstepAsyncNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockstepSessionNodePinResult, ELockstepReturn, Result);

UCLASS()
class LOCKSTEP_API ULockstepSessionAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FLockstepSessionNodePinResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FLockstepSessionNodePinResult OnFailure;

protected:
	UFUNCTION()
	void Completed(ELockstepReturn Result);

	UPROPERTY()
	ULockstepSubsystem* LockstepSubsystem;

	bool bSessionPassValid;
	ULockstepSubsystem::FSessionPass SessionPass;
};

UCLASS()
class LOCKSTEP_API URegisterLockstepSessionAction : public ULockstepSessionAction
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Lockstep|Session")
	static URegisterLockstepSessionAction* RegisterLockstepSession(UObject* WorldContextObject, const FString& IP, const FString& Port, const FString& Number, const FString& Password, bool bUseHash);

	//~ Begin UOnlineBlueprintCallProxyBase Interface
	virtual void Activate() override;
	//~ End UOnlineBlueprintCallProxyBase Interface
};

UCLASS()
class LOCKSTEP_API UJoinLockstepSessionAction : public ULockstepSessionAction
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Lockstep|Session")
	static UJoinLockstepSessionAction* JoinLockstepSession(UObject* WorldContextObject, const FString& IP, const FString& Port, const FString& Number, const FString& Password, bool bUseHash);

	//~ Begin UOnlineBlueprintCallProxyBase Interface
	virtual void Activate() override;
	//~ End UOnlineBlueprintCallProxyBase Interface
};
