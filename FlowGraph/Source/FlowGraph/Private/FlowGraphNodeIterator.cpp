#include "FlowGraphNodeIterator.h"
#include "FlowGraph_Node.h"

void UFlowGraphNodeIterator::TickNode(const float DeltaTime)
{
	if (bMarkDirty)
	{
		return;
	}
	if (CurrentNode.IsValid() && CurrentNode.Get()->MatchTickCondition())
	{
		CurrentNode.Get()->OnTick(this, DeltaTime);
	}
}

void UFlowGraphNodeIterator::IteratorTo(UFlowGraph_Node* InNode)
{
	if (InNode == nullptr || bMarkDirty)
	{
		bMarkDirty = true;
		return;
	}
	
	if (CurrentNode.IsValid())
	{
		CurrentNode.Get()->OnNodeIteratorOut(this);
	}
	
	CurrentNode.Reset();
	
	CurrentNode = InNode;

	CurrentNode.Get()->OnNodeIteratorIn(this);
}
