// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockstepParam.h"
#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "LockstepEventSignature.generated.h"

// ��ʾ�����¼���ǩ��
USTRUCT(BlueprintType)
struct FLockstepEventSignature : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	// ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lockstep")
	TSubclassOf<ULockstepParamBase> ParamType;

	FLockstepEventSignature(TSubclassOf<ULockstepParamBase> InParamType = ULockstepParamVoid::StaticClass())
		: ParamType(InParamType)
	{ }

	virtual ~FLockstepEventSignature() { }

};
