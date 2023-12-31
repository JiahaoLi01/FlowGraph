﻿#pragma once

#include "CoreMinimal.h"
#include "Interface/FlowGraphEventListener.h"
#include "UObject/Object.h"
#include "FlowGraphSubsystem.generated.h"

class UFlowGraphInstance;
class UFlowGraphTemplate;

DECLARE_STATS_GROUP(TEXT("FlowGraph"), STATGROUP_FlowGraph, STATCAT_Advanced)
DECLARE_LOG_CATEGORY_EXTERN(LogFlowGraph, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlowGraphDelegate_Signature, UFlowGraphInstance*, FlowGraph);

USTRUCT(BlueprintType)
struct FWarpedFlowGraphInstanceArray
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UFlowGraphInstance>> InstanceArray;

	TArray<TObjectPtr<UFlowGraphInstance>> operator()()
	{
		return InstanceArray;
	}

	explicit operator TArray<TObjectPtr<UFlowGraphInstance>>() const
	{
		return InstanceArray;
	}
};

USTRUCT(BlueprintType)
struct FFlowGraphSpawnHandle
{
	GENERATED_BODY()

	FFlowGraphSpawnHandle(){}
	explicit FFlowGraphSpawnHandle(UFlowGraphInstance* InSpawnInstance) : SpawnedInstance(InSpawnInstance){}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFlowGraphInstance> SpawnedInstance;
};

UCLASS(BlueprintType, Blueprintable)
class FLOWGRAPH_API UFlowGraphSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:

	UPROPERTY()
	TArray<TObjectPtr<UFlowGraphInstance>> RegisterFlowGraph;

	UPROPERTY()
	TMap<UFlowGraphTemplate*, FWarpedFlowGraphInstanceArray> RegisterFlowGraphMap;

	UPROPERTY()
	TArray<IFlowGraphEventListener*> FlowGraphEventListeners;

public:

	UFUNCTION(BlueprintCallable)
	FFlowGraphSpawnHandle RegisterFlowGraphFromTemplate(UFlowGraphTemplate* InFlowGraph);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool UnregisterFlowGraph(UFlowGraphInstance* InFlowGraph);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool UnregisterFlowGraphById(const FName& InFlowGraphId);

public:

	UFUNCTION(BlueprintCallable)
	void PauseFlowGraphInstance(UFlowGraphInstance* InInstance) const;

	UFUNCTION(BlueprintCallable)
	void PauseFlowGraphInstancesOfGivenTemplate(UFlowGraphTemplate* InTemplate);

	UFUNCTION(BlueprintCallable)
	void ContinueFlowGraphInstance(UFlowGraphInstance* InInstance);

	UFUNCTION(BlueprintCallable)
	void ContinueFlowGraphInstancesOfGivenTemplate(UFlowGraphTemplate* InTemplate);

public:

	UFUNCTION(BlueprintCallable)
	TArray<UFlowGraphInstance*> GetInstancesFromTemplate(UFlowGraphTemplate* QueryTemplate);

	UFUNCTION(BlueprintCallable)
	UFlowGraphInstance* GetFirstInstanceFromTemplate(UFlowGraphTemplate* QueryTemplate);

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
	
	UPROPERTY(BlueprintAssignable)
	FFlowGraphDelegate_Signature OnFlowGraphPaused;

	UPROPERTY(BlueprintAssignable)
	FFlowGraphDelegate_Signature OnFlowGraphContinue;

public:

	UFUNCTION(BlueprintCallable)
	void RaiseFlowGraphEvent(UFlowGraphEventArgs* InArgs);

public:

	/** Begin FTickableGameObjectInterface */
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE virtual bool IsTickable() const override {return !IsTemplate();}
	FORCEINLINE virtual bool IsTickableInEditor() const override {return false;}
	FORCEINLINE virtual bool IsTickableWhenPaused() const override {return false;}

	FORCEINLINE virtual TStatId GetStatId() const override {RETURN_QUICK_DECLARE_CYCLE_STAT(UFlowGraphSubsystem, STATGROUP_FlowGraph)}

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	/** End FTickableGameObjectInterface */

protected:

	void TickEachActiveFlowGraph(const float DeltaTime);

	void ClearDirtyFlowGraph();
};
