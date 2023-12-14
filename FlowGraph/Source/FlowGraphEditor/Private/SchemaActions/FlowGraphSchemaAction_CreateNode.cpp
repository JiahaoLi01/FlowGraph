#include "SchemaActions/FlowGraphSchemaAction_CreateNode.h"
#include "FlowGraphSchema.h"

void FFlowGraphSchemaAction_CreateNode::SetNodeClass(const UClass* InNodeClass)
{
	NodeClass = const_cast<UClass*>(InNodeClass);
}

void FFlowGraphSchemaAction_CreateNode::SetSchema(const UFlowGraphSchema* InSchema)
{
	Schema = const_cast<UFlowGraphSchema*>(InSchema);
}

UEdGraphNode* FFlowGraphSchemaAction_CreateNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                               const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* GraphNodeTemplate = Cast<UEdGraphNode>(NodeClass->ClassDefaultObject);
	UEdGraphNode* NewGraphNode = CreateNode(ParentGraph, FromPin, Location, GraphNodeTemplate);

	if (FromPin != nullptr && Schema != nullptr)
	{
		for (UEdGraphPin* Pin : NewGraphNode->Pins)
		{
			if (Pin->Direction == EGPD_Input)
			{
				if (Schema->TryCreateConnection(FromPin, Pin))
				{
					ParentGraph->NotifyNodeChanged(FromPin->GetOwningNode());
					break;
				}
			}
		}
	}

	return nullptr;
}
