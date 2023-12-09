#pragma once

#include "CoreMinimal.h"
#include "FlowGraph_Node.h"
#include "UObject/Object.h"
#include "FlowGraphNodes_Timer.generated.h"

UCLASS()
class FLOWGRAPH_API UFlowGraphNode_Timer : public UFlowGraph_Node
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=ture))
	float TimerDuration{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float CurrentTimer = 0.0f;

public:

	UFlowGraphNode_Timer();

#if WITH_EDITOR
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeDisplayName() const override;
	virtual FText GetNodeCategory() const override;
#endif

	virtual void OnTick(UFlowGraphNodeIterator* InIterator, const float DeltaTime) override;

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator) override;
};
