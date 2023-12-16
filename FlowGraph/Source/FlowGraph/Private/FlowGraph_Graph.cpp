#include "FlowGraph_Graph.h"
#include "FlowGraphInstance.h"

UFlowGraph_Graph::UFlowGraph_Graph()
{

}

UFlowGraphTemplate* UFlowGraph_Graph::GetFlowGraphTemplate() const
{
	if (UFlowGraphTemplate* Template = Cast<UFlowGraphTemplate>(GetOuter()))
	{
		return Template;
	}
	if (const UFlowGraphInstance* Instance = Cast<UFlowGraphInstance>(GetOuter()))
	{
		return Instance->Template;
	}
	return nullptr;
}

UFlowGraphInstance* UFlowGraph_Graph::GetFlowGraphInstance() const
{
	return Cast<UFlowGraphInstance>(GetOuter());
}