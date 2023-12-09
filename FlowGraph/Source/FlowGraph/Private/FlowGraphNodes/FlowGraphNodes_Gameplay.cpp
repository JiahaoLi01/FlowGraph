#include "FlowGraphNodes/FlowGraphNodes_Gameplay.h"

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
	return NSLOCTEXT("DarkMountainEditor", "Listen For Gameplaytag Node Tooltip", "监听对应的GameplayTag事件");
}

FText UFlowGraphNode_ListenForGameplayTag::GetNodeDisplayName() const
{
	return NSLOCTEXT("DarkMountainEditor", "Listen For Gameplaytag DisplayName", "监听对应的GameplayTag事件");
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