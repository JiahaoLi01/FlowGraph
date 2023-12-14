#pragma once

#include "CoreMinimal.h"

class UFlowGraphSchema;

class FLOWGRAPHEDITOR_API FFlowGraphSchemaAction_CreateNode final : public FEdGraphSchemaAction_NewNode
{
public:

	TObjectPtr<UClass> NodeClass;

	TObjectPtr<UFlowGraphSchema> Schema;
	
public:

	FFlowGraphSchemaAction_CreateNode():FEdGraphSchemaAction_NewNode(
	NSLOCTEXT("FlowGraphEditor", "Create Node Category", "创建节点菜单"),
		NSLOCTEXT("FlowGraphEditor", "Create Node Action", "创建节点"),
	NSLOCTEXT("FlowGraphEditor", "Create Node Tooltip", "创建节点描述"),
	0
	), NodeClass(nullptr){}

	void SetNodeClass(const UClass* InNodeClass);
	void SetSchema(const UFlowGraphSchema* InSchema);

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};
