// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlowGraphEventArgs.generated.h"

/**
 * Data-Only class.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class FLOWGRAPH_API UFlowGraphEventArgs : public UObject
{
	GENERATED_BODY()
};
