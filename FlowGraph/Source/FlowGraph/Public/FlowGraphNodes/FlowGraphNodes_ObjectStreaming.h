#pragma once

#include "CoreMinimal.h"
#include "FlowGraph_Node.h"
#include "UObject/Object.h"
#include "FlowGraphNodes_ObjectStreaming.generated.h"

UCLASS()
class FLOWGRAPH_API UFlowGraphNode_EnableOrDisableActor : public UFlowGraph_Node
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enable or Disable Actor")
	TSoftObjectPtr<AActor> ActorToModify;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enable or Disable Actor")
	bool bEnable{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enable or Disable Actor")
	bool bLoadActorIfPointerNotValid{false};

public:

#if WITH_EDITOR

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeDisplayName() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeCategory() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator) override;
};

UCLASS()
class UFlowGraphNode_SpawnActorAt : public UFlowGraph_Node
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Actor At")
	TSubclassOf<AActor> ActorClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MakeEditWidget))
	FTransform SpawnTransform;

public:

#if WITH_EDITOR

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeDisplayName() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeCategory() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif

	virtual void OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator) override;
};