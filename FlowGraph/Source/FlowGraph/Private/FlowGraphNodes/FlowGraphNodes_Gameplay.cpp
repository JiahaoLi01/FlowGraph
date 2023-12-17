#include "FlowGraphNodes/FlowGraphNodes_Gameplay.h"

#include "FlowGraphInstance.h"
#include "FlowGraphNodeIterator.h"
#include "FlowGraphSubsystem.h"
#include "FlowGraphEventArgs/FlowGraphEventArgs_GameplayTags.h"

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

void UFlowGraphNode_ListenForGameplayTag::TriggerOutput(UFlowGraphNodeIterator* Iterator, const FName& InPinName) const
{
	if (UEdGraphPin* TargetPin = FindPin(InPinName, EGPD_Output))
	{
		if (!TargetPin->HasAnyConnections())
		{
			Iterator->IteratorTo(nullptr);
		}

		// Because it maybe trigger multiple output at same time, so we create iterator without deleting the iterator itself.
		// the iterator will be deleted when all of the outputs are triggered by method
		// UFlowGraphNode_ListenForGameplayTag::OnNodeIteratorIn_Implementation instead of here.
		for (int i = 0; i < TargetPin->LinkedTo.Num(); ++i)
		{
			UFlowGraph_Node* TargetNode = Cast<UFlowGraph_Node>(TargetPin->LinkedTo[i]->GetOwningNode());
			GetFlowGraphInstance()->CreateAndRegisterNodeIterator(TargetNode, UFlowGraphNodeIterator::StaticClass());
		}
	}
}
#endif

void UFlowGraphNode_ListenForGameplayTag::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	Super::OnNodeIteratorIn_Implementation(InIterator);
	GetFlowGraphInstance()->FlowGraphSubsystem->FlowGraphEventListeners.Add(this);
}

EEventHandleState UFlowGraphNode_ListenForGameplayTag::HandleFlowGraphEventArgs(UFlowGraphEventArgs* InArgs)
{
	bool bHasTriggered{false};
	if (const UFlowGraphEventArgs_GameplayTags* Arg = Cast<UFlowGraphEventArgs_GameplayTags>(InArgs))
	{
		for (const FGameplayTag ListenForTag : ListenForTags)
		{
			if (Arg->Tags.HasTag(ListenForTag))
			{
				UFlowGraphNodeIterator* Iterator = GetFlowGraphInstance()->GetIteratorAt(this);
				if (Iterator != nullptr)
				{
					bHasTriggered = true;
					TriggerOutput(Iterator, ListenForTag.GetTagName());
				}
			}
		}
	}

	if (bHasTriggered)
	{
		GetFlowGraphInstance()->RemoveNodeIteratorAt(this);
		GetFlowGraphInstance()->FlowGraphSubsystem->FlowGraphEventListeners.Remove(this);
		return EEventHandleState::Handled;
	}
	
	return EEventHandleState::Unhandled;
}

#if WITH_EDITOR
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
#endif

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
