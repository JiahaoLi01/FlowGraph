#include "SchemaActions/FlowGraphSchemaAction_CreateNode.h"

void FFlowGraphSchemaAction_CreateNode::SetNodeClass(UClass* InNodeClass)
{
	NodeClass = InNodeClass;
}

UEdGraphNode* FFlowGraphSchemaAction_CreateNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
															   const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* GraphNodeTemplate = Cast<UEdGraphNode>(NodeClass->ClassDefaultObject);
	UEdGraphNode* NewGraphNode = CreateNode(ParentGraph, FromPin, Location, GraphNodeTemplate);

	return nullptr;
}
