#include "FlowGraphTemplate.h"
#include "FlowGraphNodeIterator.h"
#include "FlowGraphSubsystem.h"
#include "FlowGraph_Graph.h"
#include "FlowGraphNodes/FlowGraphNodes_TimeSequence.h"

void UFlowGraphTemplate::Tick(const float DeltaTime)
{
	if (bIsDirty)
	{
		return;
	}
	TickIterators(DeltaTime);
	DestroyDirtyIterators();
}

void UFlowGraphTemplate::CreateAndRegisterNodeIterator(UFlowGraph_Node* InStartNode, const TSubclassOf<UFlowGraphNodeIterator>& IteratorClass)
{
	UFlowGraphNodeIterator* NewFlowGraphIterator = NewObject<UFlowGraphNodeIterator>(this, IteratorClass);
	NodeIterators.Add(NewFlowGraphIterator);
	
	NewFlowGraphIterator->OwnerGraph = this;
	NewFlowGraphIterator->IteratorTo(InStartNode);
}

void UFlowGraphTemplate::RemoveNodeIterator(UFlowGraphNodeIterator* InFlowGraphNodeIterator)
{
	InFlowGraphNodeIterator->CurrentNode = nullptr;
	NodeIterators.Remove(InFlowGraphNodeIterator);
	InFlowGraphNodeIterator->MarkAsGarbage();
}

void UFlowGraphTemplate::RemoveNodeIteratorAt(UFlowGraph_Node* InNode)
{
	UFlowGraphNodeIterator* IteratorToRemove = nullptr;

	for (UFlowGraphNodeIterator* Iterator : NodeIterators)
	{
		if (Iterator->CurrentNode.Get() == InNode)
		{
			IteratorToRemove = Iterator;
			break;
		}
	}

	RemoveNodeIterator(IteratorToRemove);
}

void UFlowGraphTemplate::TickIterators(const float DeltaTime)
{
	if (!bTickable)
	{
		return;
	}
	for (UFlowGraphNodeIterator* Iterator : NodeIterators)
	{
		Iterator->TickNode(DeltaTime);
	}
}

void UFlowGraphTemplate::DestroyDirtyIterators()
{
	NodeIterators.RemoveAll([](const TObjectPtr<UFlowGraphNodeIterator> NodeIterator)
	{
		// Assuming NodeIterator is a TObjectPtr, we must check if it is valid before dereferencing
		return NodeIterator->bMarkDirty;
	});
}

void UFlowGraphTemplate::OnRegisterToSubsystem()
{
	StartFlowGraph();
}

void UFlowGraphTemplate::OnUnregisterFromSubsystem()
{
	bTickable = false;
	bIsDirty = true;
	UE_LOG(LogFlowGraph, Log, TEXT("flow graph %s unregisted"), *GraphId.ToString())
}

void UFlowGraphTemplate::StartFlowGraph()
{
	// In development time, for debug, collect all begin nodes here.
	for (UEdGraphNode* Node : Graph->Nodes)
	{
		if (UFlowGraphNode_Begin* BeginNode = Cast<UFlowGraphNode_Begin>(Node))
		{
			BeginNodes.Add(BeginNode);
		}
	}

	for (UFlowGraphNode_Begin* BeginNode : BeginNodes)
	{
		if (BeginNode->CanStart())
		{
			CreateAndRegisterNodeIterator(BeginNode, UFlowGraphNodeIterator::StaticClass());
		}
	}
}

void UFlowGraphTemplate::ResetGraph()
{
	NodeIterators.Empty();
	// TODO : the logic here is not finished yet.
}