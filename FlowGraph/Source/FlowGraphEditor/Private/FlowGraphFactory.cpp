#include "FlowGraphFactory.h"
#include "FlowGraphTemplate.h"
#include "FlowGraphSchema.h"
#include "FlowGraph_Graph.h"
#include "Kismet2/BlueprintEditorUtils.h"

UFlowGraphFactory::UFlowGraphFactory()
{
	SupportedClass = UFlowGraphTemplate::StaticClass();
}

bool UFlowGraphFactory::CanCreateNew() const
{
	return true;
}

bool UFlowGraphFactory::ShouldShowInNewMenu() const
{
	return true;
}

UObject* UFlowGraphFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	UFlowGraphTemplate* FlowGraph = NewObject<UFlowGraphTemplate>(InParent, InClass, InName, Flags);
	UEdGraph* EdGraph = FBlueprintEditorUtils::CreateNewGraph(FlowGraph, {"FlowGraph_Graph"},
		UFlowGraph_Graph::StaticClass(), UFlowGraphSchema::StaticClass());
	FlowGraph->Graph = Cast<UFlowGraph_Graph>(EdGraph);
	return FlowGraph;
}