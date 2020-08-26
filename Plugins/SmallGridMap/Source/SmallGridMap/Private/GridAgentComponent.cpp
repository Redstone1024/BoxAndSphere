// Fill out your copyright notice in the Description page of Project Settings.


#include "GridAgentComponent.h"

#include "Engine/World.h"
#include "SmallGridMapSubsystem.h"

FIntVector UGridAgentHelper::GridDirectionToVector(EGridAgentDirection InGridDirection)
{
	switch (InGridDirection)
	{
	case EGridAgentDirection::TN: return FIntVector(1, 0, 0);
	case EGridAgentDirection::TS: return FIntVector(-1, 0, 0);
	case EGridAgentDirection::TW: return FIntVector(0, -1, 0);
	case EGridAgentDirection::TE: return FIntVector(0, 1, 0);
	case EGridAgentDirection::NW: return FIntVector(1, -1, 0);
	case EGridAgentDirection::NE: return FIntVector(1, 1, 0);
	case EGridAgentDirection::SW: return FIntVector(-1, -1, 0);
	case EGridAgentDirection::SE: return FIntVector(-1, 1, 0);
	default: checkNoEntry();
	}

	return FIntVector();
}

EGridAgentDirection UGridAgentHelper::VectorToGridDirection(FIntVector InVector)
{
	if (InVector.IsZero()) return EGridAgentDirection();
	if (InVector.X > 0 && InVector.Y == 0) return EGridAgentDirection::TN;
	if (InVector.X < 0 && InVector.Y == 0) return EGridAgentDirection::TS;
	if (InVector.X == 0 && InVector.Y < 0) return EGridAgentDirection::TW;
	if (InVector.X == 0 && InVector.Y > 0) return EGridAgentDirection::TE;
	if (InVector.X > 0 && InVector.Y < 0) return EGridAgentDirection::NW;
	if (InVector.X > 0 && InVector.Y > 0) return EGridAgentDirection::NE;
	if (InVector.X < 0 && InVector.Y < 0) return EGridAgentDirection::SW;
	if (InVector.X < 0 && InVector.Y > 0) return EGridAgentDirection::SE;

	checkNoEntry();
	return EGridAgentDirection();
}

UGridAgentComponent::UGridAgentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	GridLocation = FIntVector(-1);
	CollisionRange = { FIntVector(0) };
}

FIntVector UGridAgentComponent::GetGridLocation() const
{
	return GridSubsystem->IsValidLocation(GridLocation) ? GridLocation : FIntVector(-1);
}

bool UGridAgentComponent::SetGridLocation(FIntVector InLocation, EGridAgentClashMode ClashMode, bool bOnlyTrace)
{
	check(ClashMode == EGridAgentClashMode::Initiative || ClashMode == EGridAgentClashMode::Passive);

	if (GridLocation == InLocation) return true;

	const bool bInsideOriginal = GridSubsystem->IsValidLocation(GridLocation);
	const bool bInsideTarget = GridSubsystem->IsValidLocation(InLocation);

	// ���Ŀ������������ֻ�ǲ�ѯ
	if (!bInsideTarget && bOnlyTrace) return true;

	// ���ԭλ�ú�Ŀ���������� �򲻴���
	if (!bInsideOriginal && !bInsideTarget) return true;

	// ���Ŀ�����ڲ� ���Ŀ��λ���Ƿ�ɴ� ���ɴ�ʱ��ֱ�� return
	if (bInsideTarget)
	{
		// ���ҳ�ͻ
		int64 NodeCollisionFlags = 0;
		TArray<UGridAgentComponent*> OtherAgents;
		for (const FIntVector& CollisionOffset : CollisionRange)
		{
			const FIntVector CollisionLocation = InLocation + CollisionOffset;
			const FGridMapNode& MapNode = GridSubsystem->GetNode(CollisionLocation);
			NodeCollisionFlags |= MapNode.StaticCollisionFlags;
			NodeCollisionFlags |= MapNode.DynamicCollisionFlags;
			if (MapNode.Agent.IsValid() && MapNode.Agent != this) OtherAgents.Add(MapNode.Agent.Get());
		}

		// ��ײ��ͻ
		if (NodeCollisionFlags & CollisionFlags) return false;

		// �д����ͻ����ģʽΪ����
		if (OtherAgents.Num() && ClashMode == EGridAgentClashMode::Passive) return false;

		// ���ֻ�ǲ�ѯ�ڴ�ֱ���˳�
		if (bOnlyTrace) return true;

		// ���ģʽΪ�������Ƴ���ͻ�Ĵ���
		if (OtherAgents.Num() && ClashMode == EGridAgentClashMode::Initiative)
		{
			for (UGridAgentComponent* OtherAgent : OtherAgents)
				OtherAgent->SetGridLocation(FIntVector(-1));
		}
	}

	// ���ԭλ���ڵ�ͼ��
	if (bInsideOriginal)
	{
		// ��ԭλ��ɾ������
		for (const FIntVector& CollisionOffset : CollisionRange)
		{
			const FIntVector CollisionLocation = GridLocation + CollisionOffset;
			if (GridSubsystem->IsValidLocation(CollisionLocation))
				GridSubsystem->Agents[GridSubsystem->GetLocationIndex(CollisionLocation)] = nullptr;
		}
		GridLocation = FIntVector(-1);
	}

	// ���Ŀ���ڵ�ͼ��
	if (bInsideTarget)
	{
		// �ƶ�����Ŀ��λ��
		GridLocation = InLocation;
		for (const FIntVector& CollisionOffset : CollisionRange)
		{
			const FIntVector CollisionLocation = GridLocation + CollisionOffset;
			if (GridSubsystem->IsValidLocation(CollisionLocation))
				GridSubsystem->Agents[GridSubsystem->GetLocationIndex(CollisionLocation)] = this;
		}
	}

	// ���ԭλ���������� Ŀ����������
	if (bInsideOriginal && !bInsideTarget)
	{
		OnOutside.Broadcast();
		return true;
	}

	// ���ԭλ���������� Ŀ����������
	if (!bInsideOriginal && bInsideTarget)
	{
		OnInside.Broadcast(GridLocation);
		return true;
	}

	return true;
}

int64 UGridAgentComponent::GetCollisionFlags() const
{
	return CollisionFlags;
}

bool UGridAgentComponent::SetCollisionFlags(int64 InCollisionFlags)
{
	const bool bInside = GridSubsystem->IsValidLocation(GridLocation);
	if (bInside)
	{
		int64 NodeCollisionFlags = 0;
		for (const FIntVector& CollisionOffset : CollisionRange)
		{
			const FIntVector CollisionLocation = GridLocation + CollisionOffset;
			if (GridSubsystem->IsValidLocation(CollisionLocation))
			{
				NodeCollisionFlags |= GridSubsystem->StaticCollisionFlags[GridSubsystem->GetLocationIndex(CollisionLocation)];
				NodeCollisionFlags |= GridSubsystem->DynamicCollisionFlags[GridSubsystem->GetLocationIndex(CollisionLocation)];
			}
		}
		if (!(NodeCollisionFlags & InCollisionFlags))
		{
			CollisionFlags = InCollisionFlags;
			return true;
		}
		return false;
	}

	CollisionFlags = InCollisionFlags;
	return true;
}

const TArray<FIntVector>& UGridAgentComponent::GetCollisionRange() const
{
	return CollisionRange;
}

bool UGridAgentComponent::SetCollisionRange(const TArray<FIntVector>& InCollisionRange, EGridAgentClashMode ClashMode, bool bOnlyTrace)
{
	check(ClashMode == EGridAgentClashMode::Initiative || ClashMode == EGridAgentClashMode::Passive);

	if (!GridSubsystem->IsValidLocation(GridLocation)) return true;

	int64 NodeCollisionFlags = 0;
	TArray<UGridAgentComponent*> OtherAgents;
	for (const FIntVector& CollisionOffset : InCollisionRange)
	{
		const FIntVector CollisionLocation = GridLocation + CollisionOffset;
		const FGridMapNode& MapNode = GridSubsystem->GetNode(CollisionLocation);
		NodeCollisionFlags |= MapNode.StaticCollisionFlags;
		NodeCollisionFlags |= MapNode.DynamicCollisionFlags;
		if (MapNode.Agent.IsValid() && MapNode.Agent != this) OtherAgents.Add(MapNode.Agent.Get());
	}

	if (NodeCollisionFlags & CollisionFlags) return false;

	if (OtherAgents.Num() && ClashMode == EGridAgentClashMode::Passive) return false;

	if (bOnlyTrace) return true;

	if (OtherAgents.Num() && ClashMode == EGridAgentClashMode::Initiative)
	{
		for (UGridAgentComponent* OtherAgent : OtherAgents)
			OtherAgent->SetGridLocation(FIntVector(-1));
	}

	for (const FIntVector& CollisionOffset : CollisionRange)
	{
		const FIntVector CollisionLocation = GridLocation + CollisionOffset;
		if (GridSubsystem->IsValidLocation(CollisionLocation))
			GridSubsystem->Agents[GridSubsystem->GetLocationIndex(CollisionLocation)] = nullptr;
	}

	CollisionRange = InCollisionRange;

	for (const FIntVector& CollisionOffset : CollisionRange)
	{
		const FIntVector CollisionLocation = GridLocation + CollisionOffset;
		if (GridSubsystem->IsValidLocation(CollisionLocation))
			GridSubsystem->Agents[GridSubsystem->GetLocationIndex(CollisionLocation)] = this;
	}

	return true;
}

bool UGridAgentComponent::IsBlockPath(FIntVector Location) const
{
	int64 NodeCollisionFlags = 0;
	for (const FIntVector& CollisionOffset : CollisionRange)
	{
		const FIntVector CollisionLocation = Location + CollisionOffset;
		const FGridMapNode& MapNode = GridSubsystem->GetNode(CollisionLocation);
		NodeCollisionFlags |= MapNode.StaticCollisionFlags;
		NodeCollisionFlags |= MapNode.DynamicCollisionFlags;
	}

	return NodeCollisionFlags & CollisionFlags;
}

void UGridAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	GridSubsystem = GetWorld()->GetSubsystem<USmallGridMapSubsystem>();

	const FIntVector Target = GridLocation;
	GridLocation = FIntVector(-1);
	SetGridLocation(Target);
}
