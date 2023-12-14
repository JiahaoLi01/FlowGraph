#include "FlowGraph_Graph.h"

#include "FlowGraphInstance.h"

UFlowGraph_Graph::UFlowGraph_Graph()
{

}

UFlowGraphTemplate* UFlowGraph_Graph::GetFlowGraphTemplate() const
{
	return Cast<UFlowGraphTemplate>(GetOuter());
}

UFlowGraphInstance* UFlowGraph_Graph::GetFlowGraphInstance() const
{
	return Cast<UFlowGraphInstance>(GetOuter());
}
