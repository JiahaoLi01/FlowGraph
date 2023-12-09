#include "FlowGraphSubsystem.h"

#include "FlowGraphTemplate.h"

DECLARE_CYCLE_STAT(TEXT("FlowGraphSubsystemTick"), STAT_FlowGraphSubsystemTick, STATGROUP_FlowGraph)
DEFINE_LOG_CATEGORY(LogFlowGraph)

bool UFlowGraphSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> DerivedClasses{};
	GetDerivedClasses(StaticClass(), DerivedClasses, false);

	return DerivedClasses.Num() == 0;
}

void UFlowGraphSubsystem::RegisterFlowGraph(UFlowGraphTemplate* InFlowGraph)
{
	UFlowGraphTemplate* CopiedGraph = DuplicateObject(InFlowGraph, this);
	ActiveFlowGraph.Add(CopiedGraph);
	CopiedGraph->OnRegisterToSubsystem();
	CopiedGraph->FlowGraphSubsystem = this;

	if (OnFlowGraphRegister.IsBound())
	{
		OnFlowGraphRegister.Broadcast(CopiedGraph);
	}
}

bool UFlowGraphSubsystem::UnregisterFlowGraph(UFlowGraphTemplate* InFlowGraph)
{
	if (ActiveFlowGraph.Contains(InFlowGraph))
	{
		if (OnFlowGraphUnregister.IsBound())
		{
			OnFlowGraphUnregister.Broadcast(InFlowGraph);
		}

		InFlowGraph->OnUnregisterFromSubsystem();
		InFlowGraph->bIsDirty = true;
		
		return true;
	}
	return false;
}

bool UFlowGraphSubsystem::UnregisterFlowGraphById(const FName& InFlowGraphId)
{
	for (UPTRINT i = 0; i < ActiveFlowGraph.Num(); ++i)
	{
		if (ActiveFlowGraph[i]->GraphId == InFlowGraphId)
		{
			if (OnFlowGraphUnregister.IsBound())
			{
				OnFlowGraphUnregister.Broadcast(ActiveFlowGraph[i]);
			}

			ActiveFlowGraph[i]->OnUnregisterFromSubsystem();
			ActiveFlowGraph[i]->bIsDirty = true;
			
			return true;
		}
	}

	return false;
}

void UFlowGraphSubsystem::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_FlowGraphSubsystemTick)

	TickEachActiveFlowGraph(DeltaTime);

	ActiveFlowGraph.RemoveAll([](const TObjectPtr<UFlowGraphTemplate> FlowGraph)
	{
		return FlowGraph->bIsDirty;
	});
}

bool UFlowGraphSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || EWorldType::Game;
}

void UFlowGraphSubsystem::TickEachActiveFlowGraph(const float DeltaTime)
{
	for (UFlowGraphTemplate* FlowGraph : ActiveFlowGraph)
	{
		FlowGraph->Tick(DeltaTime);
	}
}