#include "FlowGraphNodes/FlowGraphNodes_Gameplay.h"

#include "FlowGraphSubsystem.h"

#if WITH_EDITOR
void UFlowGraphNode_ListenForGameplayTag::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	AddInputPin("In");
}

FLinearColor UFlowGraphNode_ListenForGameplayTag::GetNodeTitleColor() const
{
	return Super::GetNodeTitleColor();
}

FText UFlowGraphNode_ListenForGameplayTag::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "Listen For Gameplaytag Node Tooltip", "监听对应的GameplayTag事件");
}

FText UFlowGraphNode_ListenForGameplayTag::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "Listen For Gameplaytag DisplayName", "监听对应的GameplayTag事件");
}

FText UFlowGraphNode_ListenForGameplayTag::GetNodeCategory() const
{
	return FlowGraphTimeSequenceCategory();
}

void UFlowGraphNode_ListenForGameplayTag::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	for (const FGameplayTag& Tag : ListenForTags)
	{
		const FName TagName = Tag.GetTagName();
		if (!Outputs.Contains(TagName))
		{
			AddOutputPin(TagName);
		}
	}

	for (int i = 0; i < Outputs.Num(); ++i)
	{
		if (!ListenForTags.HasTag(FGameplayTag::RequestGameplayTag(Outputs[i])))
		{
			Outputs.RemoveAt(i);
			i--;
		}
	}
}
#endif

void UFlowGraphNode_ListenForGameplayTag::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	Super::OnNodeIteratorIn_Implementation(InIterator);
}

void UFlowGraphNode_PrintDebugInfo::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	AddInputPin("In");
	AddOutputPin("Out");
}

FLinearColor UFlowGraphNode_PrintDebugInfo::GetNodeTitleColor() const
{
	return FLinearColor::Yellow;
}

FText UFlowGraphNode_PrintDebugInfo::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "Print Debug Info Title", "打印日志");
}

FText UFlowGraphNode_PrintDebugInfo::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "Print Debug Info Display Name", "打印日志");
}

FText UFlowGraphNode_PrintDebugInfo::GetNodeCategory() const
{
	return FlowGraphDebugCategory();
}

void UFlowGraphNode_PrintDebugInfo::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	Super::OnNodeIteratorIn_Implementation(InIterator);

	if (bPrintOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, OnScreenDuration, OnScreenTextColor, Content);
	}

	UE_LOG(LogFlowGraph, Log, TEXT("%s"), *Content)
	TriggerOutput(InIterator, "Out");
}
