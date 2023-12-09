#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraphTemplate.generated.h"

class UFlowGraphSubsystem;
class UFlowGraphNode_Begin;
class UFlowGraph_Node;
class UFlowGraphNodeIterator;
class UBlackboardData;
class UFlowGraph_Graph;

UCLASS(Blueprintable, BlueprintType)
class FLOWGRAPH_API UFlowGraphTemplate : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UFlowGraph_Graph> Graph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UFlowGraphNodeIterator>> NodeIterators;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsDirty{false};

	/**
	 * @property FlowGraphSubsystem : A short-cut method to get the subsystem after the flow graph is register.
	 */
	UPROPERTY()
	TWeakObjectPtr<UFlowGraphSubsystem> FlowGraphSubsystem;

	/**
	 * @property BeginNodes : This property is used to cache all the begin nodes, finding all begins in runtime by RTTI
	 * is too slow, we cache the begin nodes with the editor so that the flow graph could start the graph immediately without
	 * need to find the begin nodes.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UFlowGraphNode_Begin>> BeginNodes;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GraphId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBlackboardData> BlackboardData;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bTickable{true};

	void Tick(const float DeltaTime);

public:

	UFUNCTION(BlueprintCallable)
	void CreateAndRegisterNodeIterator(UFlowGraph_Node* InStartNode, const TSubclassOf<UFlowGraphNodeIterator>& IteratorClass);

	UFUNCTION(BlueprintCallable)
	void RemoveNodeIterator(UFlowGraphNodeIterator* InFlowGraphNodeIterator);

	UFUNCTION(BlueprintCallable)
	void RemoveNodeIteratorAt(UFlowGraph_Node* InNode);

private:

	UFUNCTION()
	void TickIterators(const float DeltaTime);

	UFUNCTION()
	void DestroyDirtyIterators();

public:

	void OnRegisterToSubsystem();

	void OnUnregisterFromSubsystem();

	void StartFlowGraph();

	void ResetGraph();
};
