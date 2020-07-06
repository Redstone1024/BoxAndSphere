// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckerNavigation.h"
#include "DebugActor.generated.h"

UCLASS()
class BOXANDSPHERE_API ADebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADebugActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float UpdateFindPath();

	FCheckerNavigation Navigation;
	FCheckerNavigation::FPathFindingQuery Query;
	FCheckerNavigation::FPathResult Path;
};
