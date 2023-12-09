#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "FlowGraphFactory.generated.h"


UCLASS()
class FLOWGRAPHEDITOR_API UFlowGraphFactory : public UFactory
{
	GENERATED_BODY()

public:

	UFlowGraphFactory();

	virtual bool CanCreateNew() const override;

	virtual bool ShouldShowInNewMenu() const override;

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
