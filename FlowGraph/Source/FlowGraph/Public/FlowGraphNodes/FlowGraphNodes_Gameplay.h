#pragma once

#include "CoreMinimal.h"
#include "FlowGraph_Node.h"
#include "GameplayTagContainer.h"
#include "FlowGraphNodes_Gameplay.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FLOWGRAPH_API UFlowGraphNode_ListenForGameplayTag : public UFlowGraph_Node
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer ListenForTags;

#if WITH_EDITOR
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeDisplayName() const override;
	virtual FText GetNodeCategory() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator) override;
};
