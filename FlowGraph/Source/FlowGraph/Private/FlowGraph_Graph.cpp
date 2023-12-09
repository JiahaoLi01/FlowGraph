#include "FlowGraph_Graph.h"

UFlowGraph_Graph::UFlowGraph_Graph()
{

}

UFlowGraphTemplate* UFlowGraph_Graph::GetFlowGraph() const
{
	return Cast<UFlowGraphTemplate>(GetOuter());
}
