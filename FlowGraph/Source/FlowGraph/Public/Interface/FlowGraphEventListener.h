#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FlowGraphEventListener.generated.h"

class UFlowGraphEventArgs;

UENUM(Blueprintable, BlueprintType)
enum EEventHandleState
{
	Handled,
	Unhandled,
};

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UFlowGraphEventListener : public UInterface
{
	GENERATED_BODY()
};

class FLOWGRAPH_API IFlowGraphEventListener
{
	GENERATED_BODY()

public:

	virtual EEventHandleState HandleFlowGraphEventArgs(UFlowGraphEventArgs* InArgs) = 0;
};
