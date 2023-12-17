#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraph_Node.generated.h"

class UFlowGraphInstance;
class UFlowGraphNodeIterator;
class UFlowGraphTemplate;

/**
 * @class UFlowGraph_Node : A Flow Graph node can be implemented by blueprint, toggle the bUseBlueprintInfo, and then implement
 * the methods Blueprint_XXX.
 */

UCLASS(Abstract, Blueprintable, BlueprintType, meta=(FastCreateCategory="Misc|DevelopmentOnly|AbstractNode"))
class FLOWGRAPH_API UFlowGraph_Node : public UEdGraphNode
{
	GENERATED_BODY()

protected:

	inline static const FName FlowGraphNodePinCategory{"FlowGraphNodePinCategory"};
	inline static const FName FlowGraphNodeSubCategory{"FlowGraphNodeSubCategory"};

	static FText FlowGraphTimeSequenceCategory() {return NSLOCTEXT("FlowGraphEditor", "Time-Sequence Node Category", "时序");}
	static FText FlowGraphDebugCategory() {return NSLOCTEXT("FlowGraphEditor", "Debug Node Category", "调试");}
	static FText FlowGraphObjectStreamingCategory() {return NSLOCTEXT("FlowGraphEditor", "Object Streaming Node Category", "对象流送");}
	static FText FlowGraphGameplayCategory() {return NSLOCTEXT("FlowGraphEditor", "Gameplay Category", "游戏");}

public:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseBlueprintInfo{false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseBlueprintInfo==true"))
	FText BlueprintNodeTitle{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseBlueprintInfo==true"))
	FText BlueprintNodeTooltip{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseBlueprintInfo==true"))
	FText BlueprintNodeCategory{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseBlueprintInfo==true"))
	FLinearColor BlueprintNodeColor;

#endif

#if WITH_EDITOR
	
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeDisplayName() const;
	virtual FText GetNodeCategory() const;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flow Graph Nodes")
	TArray<FName> Inputs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flow Graph Nodes")
	TArray<FName> Outputs;

public:

	UFlowGraphTemplate* GetFlowGraphTemplate() const;
	UFlowGraphInstance* GetFlowGraphInstance() const;

public:

	virtual void OnTick(UFlowGraphNodeIterator* InIterator, const float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnTick")
	void Blueprint_OnTick(UFlowGraphNodeIterator* InIterator, const float DeltaTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tick")
	bool bTickable;

	UFUNCTION(BlueprintNativeEvent)
	bool MatchTickCondition();

	virtual bool MatchTickCondition_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnNodeIteratorIn(UFlowGraphNodeIterator* InIterator);

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator);

	UFUNCTION(BlueprintNativeEvent)
	void OnNodeIteratorOut(UFlowGraphNodeIterator* InIterator);

	virtual void OnNodeIteratorOut_Implementation(UFlowGraphNodeIterator* InIterator);

	UFUNCTION(BlueprintCallable)
	virtual void TriggerOutput(UFlowGraphNodeIterator* Iterator, const FName& InPinName) const;

public:

	FORCEINLINE TArray<FName> GetInputs() const {return Inputs;}
	FORCEINLINE TArray<FName> GetOutputs() const {return Outputs;}

	UFUNCTION(BlueprintCallable)
	bool AddInputPin(const FName& InPinName);

	UFUNCTION(BlueprintCallable)
	bool RemoveInputPin(const FName InPinName);

	UFUNCTION(BlueprintCallable)
	bool AddOutputPin(const FName& InPinName);

	UFUNCTION(BlueprintCallable)
	bool RemoveOutputPin(const FName& InPinName);

	UFUNCTION(BlueprintCallable)
	UFlowGraphInstance* GetFlowGraphInstance();

	UFUNCTION(BlueprintCallable)
	UFlowGraphTemplate* GetFlowGraphTemplate();
};