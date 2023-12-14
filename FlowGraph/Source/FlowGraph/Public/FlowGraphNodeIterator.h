#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraphNodeIterator.generated.h"

class UFlowGraphInstance;
class UFlowGraph_Node;
class UFlowGraphTemplate;

UCLASS(BlueprintType, Blueprintable)
class FLOWGRAPH_API UFlowGraphNodeIterator : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Iterator")
	TWeakObjectPtr<UFlowGraphInstance> OwnerGraph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Iterator")
	TWeakObjectPtr<UFlowGraph_Node> CurrentNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Iterator")
	bool bMarkDirty{false};

public:

	UFUNCTION()
	void TickNode(const float DeltaTime);

public:

	UFUNCTION()
	void IteratorTo(UFlowGraph_Node* InNode);
};