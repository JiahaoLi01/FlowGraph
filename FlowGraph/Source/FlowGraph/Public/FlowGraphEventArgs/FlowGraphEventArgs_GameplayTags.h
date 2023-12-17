#pragma once

#include "CoreMinimal.h"
#include "FlowGraphEventArgs.h"
#include "GameplayTagContainer.h"
#include "FlowGraphEventArgs_GameplayTags.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FLOWGRAPH_API UFlowGraphEventArgs_GameplayTags : public UFlowGraphEventArgs
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags;
};
