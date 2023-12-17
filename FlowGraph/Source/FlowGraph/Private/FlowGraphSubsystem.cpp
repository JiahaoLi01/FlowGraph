#include "FlowGraphSubsystem.h"

#include "FlowGraphInstance.h"
#include "FlowGraphTemplate.h"

DECLARE_CYCLE_STAT(TEXT("FlowGraphSubsystemTick"), STAT_FlowGraphSubsystemTick, STATGROUP_FlowGraph)
DEFINE_LOG_CATEGORY(LogFlowGraph)

bool UFlowGraphSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> DerivedClasses{};
	GetDerivedClasses(StaticClass(), DerivedClasses, false);

	return DerivedClasses.Num() == 0;
}

FFlowGraphSpawnHandle UFlowGraphSubsystem::RegisterFlowGraphFromTemplate(UFlowGraphTemplate* InFlowGraph)
{
	UFlowGraphInstance* Instance = InFlowGraph->SpawnInstance(this);
	RegisterFlowGraph.Add(Instance);
	Instance->OnRegisterToSubsystem();
	Instance->FlowGraphSubsystem = this;

	if (OnFlowGraphRegister.IsBound())
	{
		OnFlowGraphRegister.Broadcast(Instance);
	}

	if (RegisterFlowGraphMap.Contains(InFlowGraph))
	{
		RegisterFlowGraphMap[InFlowGraph].InstanceArray.Add(Instance);
	}
	else
	{
		RegisterFlowGraphMap.Add(InFlowGraph);
		RegisterFlowGraphMap[InFlowGraph].InstanceArray.Add(Instance);
	}

	const FFlowGraphSpawnHandle Handle(Instance);
	return Handle;
}

bool UFlowGraphSubsystem::UnregisterFlowGraph(UFlowGraphInstance* InFlowGraph)
{
	if (RegisterFlowGraph.Contains(InFlowGraph))
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
	for (UPTRINT i = 0; i < RegisterFlowGraph.Num(); ++i)
	{
		if (RegisterFlowGraph[i]->GraphId == InFlowGraphId)
		{
			if (OnFlowGraphUnregister.IsBound())
			{
				OnFlowGraphUnregister.Broadcast(RegisterFlowGraph[i]);
			}

			RegisterFlowGraph[i]->OnUnregisterFromSubsystem();
			RegisterFlowGraph[i]->bIsDirty = true;
			
			return true;
		}
	}

	return false;
}

void UFlowGraphSubsystem::PauseFlowGraphInstance(UFlowGraphInstance* InInstance) const
{
	InInstance->bTickIsPause = true;
	if (OnFlowGraphPaused.IsBound())
	{
		OnFlowGraphPaused.Broadcast(InInstance);
	}
}

void UFlowGraphSubsystem::PauseFlowGraphInstancesOfGivenTemplate(UFlowGraphTemplate* InTemplate)
{
	for (UFlowGraphInstance* Instance : RegisterFlowGraphMap[InTemplate].InstanceArray)
	{
		Instance->bTickIsPause = true;

		if (OnFlowGraphPaused.IsBound())
		{
			OnFlowGraphPaused.Broadcast(Instance);
		}
	}
}

void UFlowGraphSubsystem::ContinueFlowGraphInstance(UFlowGraphInstance* InInstance)
{
	InInstance->bTickIsPause = false;
	if (OnFlowGraphContinue.IsBound())
	{
		OnFlowGraphContinue.Broadcast(InInstance);
	}
}

void UFlowGraphSubsystem::ContinueFlowGraphInstancesOfGivenTemplate(UFlowGraphTemplate* InTemplate)
{
	for (UFlowGraphInstance* Instance : RegisterFlowGraphMap[InTemplate].InstanceArray)
	{
		Instance->bTickIsPause = false;

		if (OnFlowGraphContinue.IsBound())
		{
			OnFlowGraphContinue.Broadcast(Instance);
		}
	}
}

TArray<UFlowGraphInstance*> UFlowGraphSubsystem::GetInstancesFromTemplate(UFlowGraphTemplate* QueryTemplate)
{
	if (RegisterFlowGraphMap.Contains(QueryTemplate) && RegisterFlowGraphMap[QueryTemplate]().Num() > 0)
	{
		return RegisterFlowGraphMap[QueryTemplate]();
	}
	return {};
}

UFlowGraphInstance* UFlowGraphSubsystem::GetFirstInstanceFromTemplate(UFlowGraphTemplate* QueryTemplate)
{
	if (RegisterFlowGraphMap.Contains(QueryTemplate) && RegisterFlowGraphMap[QueryTemplate]().Num() > 0)
	{
		return RegisterFlowGraphMap[QueryTemplate]()[0];
	}
	return nullptr;
}

void UFlowGraphSubsystem::RaiseFlowGraphEvent(UFlowGraphEventArgs* InArgs)
{
	for (auto InterfaceIterator = FlowGraphEventListeners.CreateIterator(); InterfaceIterator; ++InterfaceIterator)
	{
		(*InterfaceIterator)->HandleFlowGraphEventArgs(InArgs);
	}
}

void UFlowGraphSubsystem::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_FlowGraphSubsystemTick)

	TickEachActiveFlowGraph(DeltaTime);
	ClearDirtyFlowGraph();
}

bool UFlowGraphSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || EWorldType::Game;
}

void UFlowGraphSubsystem::TickEachActiveFlowGraph(const float DeltaTime)
{
	for (UFlowGraphInstance* FlowGraph : RegisterFlowGraph)
	{
		FlowGraph->Tick(DeltaTime);
	}
}

void UFlowGraphSubsystem::ClearDirtyFlowGraph()
{
	TArray<UFlowGraphTemplate*> EmptyInstancesMarkList{};
	
	for (auto Pair : RegisterFlowGraphMap)
	{
		auto& [InstanceArray] = Pair.Value;
		InstanceArray.RemoveAll([](const TObjectPtr<UFlowGraphInstance> FlowGraph)
		{
			return FlowGraph->bIsDirty;
		});
		if (InstanceArray.Num() == 0)
		{
			EmptyInstancesMarkList.Add(Pair.Key);
		}
	}

	for (const UFlowGraphTemplate* Template : EmptyInstancesMarkList)
	{
		RegisterFlowGraphMap.Remove(Template);
	}
	
	RegisterFlowGraph.RemoveAll([](const TObjectPtr<UFlowGraphInstance> FlowGraph)
	{
		return FlowGraph->bIsDirty;
	});
}
