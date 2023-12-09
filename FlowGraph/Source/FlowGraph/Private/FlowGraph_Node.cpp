﻿#include "FlowGraph_Node.h"
#include "FlowGraphNodeIterator.h"
#include "FlowGraphSubsystem.h"
#include "FlowGraph_Graph.h"

#if WITH_EDITOR

void UFlowGraph_Node::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	for (const FName& Input : Inputs)
	{
		CreatePin(EGPD_Input, FlowGraphNodePinCategory, Input);
	}

	for (const FName& Output : Outputs)
	{
		CreatePin(EGPD_Output, FlowGraphNodeSubCategory, Output);
	}
}

FLinearColor UFlowGraph_Node::GetNodeTitleColor() const
{
	if (bUseBlueprintInfo)
	{
		return BlueprintNodeColor;
	}
	return FLinearColor::Red;
}

FText UFlowGraph_Node::GetTooltipText() const
{
	if (bUseBlueprintInfo)
	{
		return BlueprintNodeTooltip;
	}
	return NSLOCTEXT("DarkMountainEditor", "FlowGraphBaseNodeTooltip", "流图基类节点，用于基类，理论上讲不可创建");
}

FText UFlowGraph_Node::GetNodeDisplayName() const
{
	if (bUseBlueprintInfo)
	{
		return BlueprintNodeTitle;
	}
	return NSLOCTEXT("DarkMountainEditor", "FlowGraphBaseNodeTitle", "流图基类节点");
}

FText UFlowGraph_Node::GetNodeCategory() const
{
	if (bUseBlueprintInfo)
	{
		return BlueprintNodeCategory;
	}
	return NSLOCTEXT("DarkMountainEditor", "FlowGraphBaseNodeCategory", "创建新节点|测试节点");
}

void UFlowGraph_Node::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Inputs)
		|| PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Outputs))
	{
		Pins.Empty();
		UFlowGraph_Node::AllocateDefaultPins();
		GetGraph()->NotifyNodeChanged(this);
	}
}

FText UFlowGraph_Node::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetNodeDisplayName();
}
#endif

UFlowGraphTemplate* UFlowGraph_Node::GetFlowGraph() const
{
	if (const UFlowGraph_Graph* Graph = Cast<UFlowGraph_Graph>(GetGraph()); Graph != nullptr)
	{
		return Graph->GetFlowGraph();
	}
	return nullptr;
}

void UFlowGraph_Node::OnTick(UFlowGraphNodeIterator* InIterator, const float DeltaTime)
{
	Blueprint_OnTick(InIterator, DeltaTime);
}

bool UFlowGraph_Node::MatchTickCondition_Implementation()
{
	return bTickable;
}

void UFlowGraph_Node::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	UE_LOG(LogFlowGraph, Log, TEXT("A iterator is out from node %s"), *GetName());
}

void UFlowGraph_Node::OnNodeIteratorOut_Implementation(UFlowGraphNodeIterator* InIterator)
{
	UE_LOG(LogFlowGraph, Log, TEXT("A iterator comes to the node %s"), *GetName());
}

void UFlowGraph_Node::TriggerOutput(UFlowGraphNodeIterator* Iterator, const FName& InPinName)
{
	if (UEdGraphPin* TargetPin = FindPin(InPinName, EGPD_Output))
	{
		if (!TargetPin->HasAnyConnections())
		{
			Iterator->IteratorTo(nullptr);
		}
		
		if (TargetPin->LinkedTo.Num() == 1)
		{
			UFlowGraph_Node* FlowGraphNode = Cast<UFlowGraph_Node>(TargetPin->LinkedTo[0]->GetOwningNode());
			Iterator->IteratorTo(FlowGraphNode);
		}
		else
		{
			// TODO : Logic when it has multiple links.
		}
	}
}

bool UFlowGraph_Node::AddInputPin(const FName& InPinName)
{
	if (FindPin(InPinName, EGPD_Input) != nullptr)
	{
		return false;
	}
	CreatePin(EGPD_Input, FlowGraphNodePinCategory, FlowGraphNodeSubCategory, InPinName);
	Inputs.Add(InPinName);
	GetGraph()->NotifyNodeChanged(this);
	return true;
}

bool UFlowGraph_Node::RemoveInputPin(const FName InPinName)
{
	UEdGraphPin* FoundPin = FindPin(InPinName, EGPD_Input);
	if (FoundPin != nullptr)
	{
		return false;
	}
	RemovePin(FoundPin);
	Inputs.Remove(InPinName);
	GetGraph()->NotifyNodeChanged(this);
	return true;
}

bool UFlowGraph_Node::AddOutputPin(const FName& InPinName)
{
	if (FindPin(InPinName, EGPD_Output) != nullptr)
	{
		return false;
	}
	CreatePin(EGPD_Output, FlowGraphNodePinCategory, FlowGraphNodeSubCategory, InPinName);
	Outputs.Add(InPinName);
	GetGraph()->NotifyNodeChanged(this);
	return true;
}

bool UFlowGraph_Node::RemoveOutputPin(const FName& InPinName)
{
	UEdGraphPin* FoundPin = FindPin(InPinName, EGPD_Output);
	if (FoundPin != nullptr)
	{
		return false;
	}
	RemovePin(FoundPin);
	Outputs.Remove(InPinName);
	GetGraph()->NotifyNodeChanged(this);
	return true;
}
