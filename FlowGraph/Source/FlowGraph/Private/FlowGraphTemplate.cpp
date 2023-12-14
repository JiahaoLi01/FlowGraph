#include "FlowGraphTemplate.h"
#include "FlowGraphInstance.h"
#include "FlowGraphSubsystem.h"


UFlowGraphInstance* UFlowGraphTemplate::SpawnInstance(UObject* Outer)
{
	UFlowGraphInstance* FlowGraphInstance = NewObject<UFlowGraphInstance>(Outer);
	FlowGraphInstance->DuplicateDataFromTemplate(this);

	return FlowGraphInstance;
}

TArray<UFlowGraphInstance*> UFlowGraphTemplate::GetInstancesOfThisTemplate() const
{
	TArray<UFlowGraphInstance*> FlowGraphInstances{};

	if (UFlowGraphSubsystem* FlowGraphSubsystem = GetWorld()->GetSubsystem<UFlowGraphSubsystem>())
	{
		FlowGraphInstances = FlowGraphSubsystem->RegisterFlowGraphMap[this]();
	}
	
	return FlowGraphInstances;
}
