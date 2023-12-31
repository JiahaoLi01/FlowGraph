﻿#include "FlowGraphInstance.h"
#include "FlowGraphNodeIterator.h"
#include "FlowGraphSubsystem.h"
#include "FlowGraphTemplate.h"
#include "FlowGraph_Graph.h"
#include "FlowGraphNodes/FlowGraphNodes_TimeSequence.h"
#include "BehaviorTree/BlackboardData.h"

UFlowGraphInstance::UFlowGraphInstance()
{
}

void UFlowGraphInstance::DuplicateDataFromTemplate(UFlowGraphTemplate* InFlowGraphTemplate)
{
	Template = InFlowGraphTemplate;
	Graph = DuplicateObject(Template->Graph, this, FName{TEXTVIEW("Graph")});

	if (Template->bUseIndividualBlackboardInEachInstance)
	{
		BlackboardData = DuplicateObject(Template->BlackboardData, this, FName{TEXTVIEW("BlackboardData")});
	}
	else
	{
		BlackboardData = Template->BlackboardData;
	}
}

void UFlowGraphInstance::Tick(const float DeltaTime)
{
	if (bIsDirty || bTickIsPause)
	{
		return;
	}
	TickIterators(DeltaTime);
	DestroyDirtyIterators();

	if (Template->bUnregisterIfNoIterator)
	{
		UnregisterSelfIfNoIterators();
	}
}

void UFlowGraphInstance::CreateAndRegisterNodeIterator(UFlowGraph_Node* InStartNode, const TSubclassOf<UFlowGraphNodeIterator>& IteratorClass)
{
	UFlowGraphNodeIterator* NewFlowGraphIterator = NewObject<UFlowGraphNodeIterator>(this, IteratorClass);
	NodeIterators.Add(NewFlowGraphIterator);
	
	NewFlowGraphIterator->OwnerGraph = this;
	NewFlowGraphIterator->IteratorTo(InStartNode);
}

void UFlowGraphInstance::RemoveNodeIterator(UFlowGraphNodeIterator* InFlowGraphNodeIterator)
{
	InFlowGraphNodeIterator->CurrentNode = nullptr;
	NodeIterators.Remove(InFlowGraphNodeIterator);
	InFlowGraphNodeIterator->MarkAsGarbage();
}

void UFlowGraphInstance::RemoveNodeIteratorAt(UFlowGraph_Node* InNode)
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

	if (IteratorToRemove != nullptr)
	{
		RemoveNodeIterator(IteratorToRemove);
	}
}

void UFlowGraphInstance::RemoveAllIterator()
{
	NodeIterators.Empty();
}

UFlowGraphNodeIterator* UFlowGraphInstance::GetIteratorAt(UFlowGraph_Node* InNode)
{
	for (UFlowGraphNodeIterator* Iterator : NodeIterators)
	{
		if (Iterator->CurrentNode == InNode)
		{
			return Iterator;
		}
	}

	return nullptr;
}

void UFlowGraphInstance::SetPause(const bool NewState)
{
	SetPauseWithoutDelegateBroadcast(NewState);
	
	if (NewState && FlowGraphSubsystem->OnFlowGraphPaused.IsBound())
	{
		FlowGraphSubsystem->OnFlowGraphPaused.Broadcast(this);
	}
	else if (!NewState && FlowGraphSubsystem->OnFlowGraphContinue.IsBound())
	{
		FlowGraphSubsystem->OnFlowGraphContinue.Broadcast(this);
	}
	
}

void UFlowGraphInstance::TickIterators(const float DeltaTime)
{
	for (UFlowGraphNodeIterator* Iterator : NodeIterators)
	{
		Iterator->TickNode(DeltaTime);
	}
}

void UFlowGraphInstance::DestroyDirtyIterators()
{
	NodeIterators.RemoveAll([](const TObjectPtr<UFlowGraphNodeIterator> NodeIterator)
	{
		// Assuming NodeIterator is a TObjectPtr, we must check if it is valid before dereferencing
		return NodeIterator->bMarkDirty;
	});
}

void UFlowGraphInstance::UnregisterSelfIfNoIterators()
{
	if (NodeIterators.Num() == 0)
	{
		FlowGraphSubsystem->UnregisterFlowGraph(this);
	}
}


void UFlowGraphInstance::OnRegisterToSubsystem()
{
	StartFlowGraph();
}

void UFlowGraphInstance::OnUnregisterFromSubsystem()
{
	bIsDirty = true;
	UE_LOG(LogFlowGraph, Log, TEXT("flow graph %s unregisted"), *GraphId.ToString())
}

void UFlowGraphInstance::StartFlowGraph()
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

void UFlowGraphInstance::ResetGraph()
{
	RemoveAllIterator();

	if (Template->bUseIndividualBlackboardInEachInstance)
	{
		BlackboardData = DuplicateObject(Template->BlackboardData, this, FName{TEXTVIEW("BlackboardData")});
	}

	StartFlowGraph();
}