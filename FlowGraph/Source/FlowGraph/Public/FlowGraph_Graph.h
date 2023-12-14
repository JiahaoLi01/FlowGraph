#pragma once

#include "CoreMinimal.h"
#include "FlowGraphTemplate.h"
#include "FlowGraph_Graph.generated.h"

class UFlowGraph_Node;
class UFlowGraphNodeIterator;
class UFlowGraph;

UCLASS()
class FLOWGRAPH_API UFlowGraph_Graph : public UEdGraph
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<UFlowGraphTemplate> FlowGraph;

public:

	UFlowGraph_Graph();

	UFlowGraphTemplate* GetFlowGraphTemplate() const;

	UFlowGraphInstance* GetFlowGraphInstance() const;
};
