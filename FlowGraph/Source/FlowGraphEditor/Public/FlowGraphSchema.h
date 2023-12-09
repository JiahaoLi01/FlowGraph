#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "FlowGraphSchema.generated.h"

UCLASS()
class FLOWGRAPHEDITOR_API UFlowGraphSchema : public UEdGraphSchema
{
GENERATED_BODY()

public:

virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
};