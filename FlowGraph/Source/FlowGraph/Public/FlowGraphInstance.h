#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraphInstance.generated.h"

class UFlowGraph_Node;
class UBlackboardData;
class UFlowGraphNode_Begin;
class UFlowGraphSubsystem;
class UFlowGraphNodeIterator;
class UFlowGraph_Graph;
class UFlowGraphTemplate;

UCLASS(BlueprintType)
class FLOWGRAPH_API UFlowGraphInstance : public UObject
{
	GENERATED_BODY()

	friend class UFlowGraphSubsystem;

public:

	UPROPERTY()
	TObjectPtr<UFlowGraph_Graph> Graph;

	UPROPERTY()
	TObjectPtr<UFlowGraphTemplate> Template;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UFlowGraphNodeIterator>> NodeIterators;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsDirty{false};

	/**
	 * @property bTickIsPause : when it is set to true, the subsystem will ignore its tick function, so that all tick logic
	 * will be paused, the timer for example. However, nodes that are driven by event will not be influenced.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bTickIsPause{false};

public:

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

	UFlowGraphInstance();

public:

	UFUNCTION(BlueprintCallable)
	void DuplicateDataFromTemplate(UFlowGraphTemplate* InFlowGraphTemplate);

public:

	void Tick(const float DeltaTime);

public:

	UFUNCTION(BlueprintCallable)
	void CreateAndRegisterNodeIterator(UFlowGraph_Node* InStartNode, const TSubclassOf<UFlowGraphNodeIterator>& IteratorClass);

	UFUNCTION(BlueprintCallable)
	void RemoveNodeIterator(UFlowGraphNodeIterator* InFlowGraphNodeIterator);

	UFUNCTION(BlueprintCallable)
	void RemoveNodeIteratorAt(UFlowGraph_Node* InNode);

	UFUNCTION(BlueprintCallable)
	void RemoveAllIterator();

	UFUNCTION(BlueprintCallable)
	UFlowGraphNodeIterator* GetIteratorAt(UFlowGraph_Node* InNode);

	/**
	 * @function SetPauseWithoutDelegateBroadcast : This method will not invoke the multicast delegate in the subsystem.
	 * @param NewState 
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPauseWithoutDelegateBroadcast(const bool NewState) {bTickIsPause = NewState;}

	UFUNCTION(BlueprintCallable)
	void SetPause(const bool NewState);

private:

	UFUNCTION()
	void TickIterators(const float DeltaTime);

	UFUNCTION()
	void DestroyDirtyIterators();

	void UnregisterSelfIfNoIterators();

public:

	void OnRegisterToSubsystem();

	void OnUnregisterFromSubsystem();

	void StartFlowGraph();

	void ResetGraph();
};
