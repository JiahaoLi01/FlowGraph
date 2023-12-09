#pragma once

#include "CoreMinimal.h"
#include "FlowGraph_Node.h"
#include "UObject/Object.h"
#include "FlowGraphNodes_TimeSequence.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FLOWGRAPH_API UFlowGraphNode_Begin : public UFlowGraph_Node
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeDisplayName() const override;
	virtual FText GetNodeCategory() const override;
#endif

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator) override;

public:

	UFUNCTION(BlueprintNativeEvent)
	bool CanStart();

	virtual bool CanStart_Implementation() {return true;}
};

UCLASS(Blueprintable, BlueprintType)
class FLOWGRAPH_API UFlowGraphNode_End : public UFlowGraph_Node
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeDisplayName() const override;
	virtual FText GetNodeCategory() const override;
#endif

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator) override;
};