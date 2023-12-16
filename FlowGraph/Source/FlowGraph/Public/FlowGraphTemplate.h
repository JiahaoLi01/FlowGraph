#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraphTemplate.generated.h"

class UFlowGraphInstance;
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

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GraphId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseIndividualBlackboardInEachInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBlackboardData> BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnregisterIfNoIterator{true};

public:

	// Template methods.
	UFUNCTION(BlueprintCallable)
	UFlowGraphInstance* SpawnInstance(UObject* Outer);
	
	UFUNCTION(BlueprintCallable)
	TArray<UFlowGraphInstance*> GetInstancesOfThisTemplate() const;
};
