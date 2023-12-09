#include "FlowGraphSchema.h"
#include "FlowGraph_Node.h"
#include "Framework/Commands/GenericCommands.h"
#include "SchemaActions/FlowGraphSchemaAction_CreateNode.h"

void UFlowGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const UClass* BaseNodeClass = UFlowGraph_Node::StaticClass();
	
	TArray<UClass*> NodeClasses;
	GetDerivedClasses(BaseNodeClass, NodeClasses, true);
	
	for (const auto& NodeClassIterator : NodeClasses)
	{
		const TSharedPtr<FFlowGraphSchemaAction_CreateNode> CreateNodeAction = MakeShareable(new FFlowGraphSchemaAction_CreateNode);
		const UFlowGraph_Node* NodeCDO = Cast<UFlowGraph_Node>(NodeClassIterator->ClassDefaultObject);

		static const FName FastCategoryMetaKey {"FastCreateCategory"};

		if (!NodeClassIterator->HasMetaData(FastCategoryMetaKey))
		{
			CreateNodeAction->UpdateSearchData(NodeCDO->GetNodeDisplayName(), FText(),
			NodeCDO->GetNodeCategory(), FText());
		}
		else
		{
			const FText& CategoryText = FText::FromString(NodeClassIterator->GetMetaData(FastCategoryMetaKey));
			CreateNodeAction->UpdateSearchData(NodeCDO->GetNodeDisplayName(), FText(),
			CategoryText, FText());
		}
		CreateNodeAction->SetNodeClass(NodeClassIterator);
		ContextMenuBuilder.AddAction(CreateNodeAction);
	}
	
	Super::GetGraphContextActions(ContextMenuBuilder);
}

void UFlowGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	static const FName FlowGraphContextMenuSection {"FlowGraphContextMenuSection"};
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection(FlowGraphContextMenuSection, NSLOCTEXT("DarkMountainEditor", "NodeContextMenuSection", "节点操作菜单"));
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Paste);
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
	}
	if (Context->Pin)
	{
		FToolMenuSection& Section = Menu->AddSection(FlowGraphContextMenuSection, NSLOCTEXT("DarkMountainEditor", "PinMenuSection", "引脚菜单"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
	}
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UFlowGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pins with same direction"));
	}
	if (A->GetOwningNode() != B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("OK!"));
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect the output pin to the input pin"));
}