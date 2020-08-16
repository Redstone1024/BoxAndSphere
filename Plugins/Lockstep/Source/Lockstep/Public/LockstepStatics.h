// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockstepSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LockstepStatics.generated.h"

UCLASS()
class LOCKSTEP_API ULockstepStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static ELockstepNetMode GetLockstepNetMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static bool SetLockstepNetMode(const UObject* WorldContextObject, ELockstepNetMode NewMode);

	UFUNCTION(BlueprintPure, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static ELockstepStatus GetLockstepStatus(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static void GetLockstepSessionPass(const UObject* WorldContextObject, FString& IP, FString& Port, FString& Number, FString& Password);

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static void PrintLogOnLockstepServer(const UObject* WorldContextObject, const FString& InString = FString(TEXT("Hello")));

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject", CustomStructureParam = "EventStruct", AutoCreateRefTerm = "EventStruct"), CustomThunk)
	static void SendLockstepEvent(const UObject* WorldContextObject, const int32& EventStruct) { }
	static void Generic_SendLockstepEvent(const UObject* WorldContextObject, UScriptStruct* EventType, void* EventParams, const FString& StackTraceStr);
	DECLARE_FUNCTION(execSendLockstepEvent)
	{
		P_GET_OBJECT(UObject, WorldContextObject);

		Stack.Step(Stack.Object, nullptr);
		void* StructPtr = Stack.MostRecentPropertyAddress;
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);

		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_SendLockstepEvent(WorldContextObject, StructProperty ? StructProperty->Struct : nullptr, StructPtr, Stack.GetStackTrace());
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static void BindTickDelegate(const UObject* WorldContextObject, FLockstepTickDynamicDelegate Event);

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static void BindEventDelegate(const UObject* WorldContextObject, UScriptStruct* EventType, FLockstepEventDynamicDelegate Event);

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static void BindErrorDelegate(const UObject* WorldContextObject, UScriptStruct* EventType, FLockstepErrorDynamicDelegate Event);

	UFUNCTION(BlueprintPure, Category = "Lockstep", meta = (CustomStructureParam = "Event", AutoCreateRefTerm = "Event"), CustomThunk)
	static void DereferenceEventPtr(const FLockstepEventPtr& EventPtr, int32& Event, bool& bSuccess) { }
	static void Generic_DereferenceEventPtr(const FLockstepEventPtr& EventPtr, UScriptStruct* EventType, void* Event, bool& bSuccess);
	DECLARE_FUNCTION(execDereferenceEventPtr)
	{
		P_GET_STRUCT(FLockstepEventPtr, EventStructPtr);

		Stack.Step(Stack.Object, nullptr);
		void* StructPtr = Stack.MostRecentPropertyAddress;
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);

		P_GET_UBOOL_REF(bSuccess);

		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_DereferenceEventPtr(EventStructPtr, StructProperty ? StructProperty->Struct : nullptr, StructPtr, bSuccess);
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintCallable, Category = "Lockstep", meta = (WorldContext = "WorldContextObject"))
	static void ResetLockstepSubsystem(const UObject* WorldContextObject);
};
