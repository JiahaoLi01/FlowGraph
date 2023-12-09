#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraphSubsystem.generated.h"

DECLARE_STATS_GROUP(TEXT("FlowGraph"), STATGROUP_FlowGraph, STATCAT_Advanced)

class UFlowGraphTemplate;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlowGraphDelegate_Signature, UFlowGraphTemplate*, FlowGraph);
DECLARE_LOG_CATEGORY_EXTERN(LogFlowGraph, Log, All);

UCLASS(BlueprintType, Blueprintable)
class FLOWGRAPH_API UFlowGraphSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:

	UPROPERTY()
	TArray<TObjectPtr<UFlowGraphTemplate>> ActiveFlowGraph;

public:

	UFUNCTION(BlueprintCallable)
	void RegisterFlowGraph(UFlowGraphTemplate* InFlowGraph);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool UnregisterFlowGraph(UFlowGraphTemplate* InFlowGraph);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool UnregisterFlowGraphById(const FName& InFlowGraphId);

	/**
	 * @brief It is called after the flow graph object is added to the active flow graph list, and after calling the method
	 * OnRegisterToSubsystem.
	 */
	UPROPERTY(BlueprintAssignable)
	FFlowGraphDelegate_Signature OnFlowGraphRegister;

	/**
	 * @brief Actually, it is called before the flow graph object has been removed from the active flow graph list,
	 * before calling the method OnUnregisterFromSubsystem and before the object is marked as GC-dirty.
	 * So that programmers can avoid the GC problems.
	 */
	UPROPERTY(BlueprintAssignable)
	FFlowGraphDelegate_Signature OnFlowGraphUnregister;

public:

	virtual void Tick(float DeltaTime) override;
	FORCEINLINE virtual bool IsTickable() const override {return !IsTemplate();}
	FORCEINLINE virtual bool IsTickableInEditor() const override {return false;}
	FORCEINLINE virtual bool IsTickableWhenPaused() const override {return false;}

	FORCEINLINE virtual TStatId GetStatId() const override {RETURN_QUICK_DECLARE_CYCLE_STAT(UFlowGraphSubsystem, STATGROUP_FlowGraph)}

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

protected:

	void TickEachActiveFlowGraph(const float DeltaTime);
};
