// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockstepParam.h"
#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "LockstepEventSignature.generated.h"

// 表示锁步事件的签名
USTRUCT(BlueprintType)
struct FLockstepEventSignature : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	// 参数类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	TSubclassOf<ULockstepParamBase> ParamType;

	FLockstepEventSignature(TSubclassOf<ULockstepParamBase> InParamType = ULockstepParamVoid::StaticClass())
		: ParamType(InParamType)
	{ }

	virtual ~FLockstepEventSignature() { }

};
