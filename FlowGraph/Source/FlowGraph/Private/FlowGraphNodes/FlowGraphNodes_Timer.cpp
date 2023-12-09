#include "FlowGraphNodes/FlowGraphNodes_Timer.h"
#include "FlowGraphSubsystem.h"

#if WITH_EDITOR

void UFlowGraphNode_Timer::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	AddInputPin("In");
	AddOutputPin("Output");
}

FLinearColor UFlowGraphNode_Timer::GetNodeTitleColor() const
{
	return Super::GetNodeTitleColor();
}

FText UFlowGraphNode_Timer::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "Timer Node Tooltip", "计时器，使用倒计时");
}

FText UFlowGraphNode_Timer::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "Timer Node Title", "计时器");
}

FText UFlowGraphNode_Timer::GetNodeCategory() const
{
	return FlowGraphTimeSequenceCategory();
}

#endif

UFlowGraphNode_Timer::UFlowGraphNode_Timer()
{
	bTickable = true;
}

void UFlowGraphNode_Timer::OnTick(UFlowGraphNodeIterator* InIterator, const float DeltaTime)
{
	Super::OnTick(InIterator, DeltaTime);

	CurrentTimer += DeltaTime;

	if (CurrentTimer >= TimerDuration)
	{
		TriggerOutput(InIterator, "Output");
		CurrentTimer = 0.0f;
		UE_LOG(LogFlowGraph, Log, TEXT("Timer out"))
	}
}

void UFlowGraphNode_Timer::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	Super::OnNodeIteratorIn_Implementation(InIterator);
	CurrentTimer = 0.0f;
}