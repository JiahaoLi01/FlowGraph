#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "FlowGraphTemplate.h"
#include "IAssetTools.h"
#include "AssetDefinition_FlowGraph.generated.h"

class FAssetToolsModule;

UCLASS()
class FLOWGRAPHEDITOR_API UAssetDefinition_FlowGraph : public UAssetDefinitionDefault
{
	GENERATED_BODY()

	// UAssetDefinition Begin
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_FlowGraph", "流图"); }
	virtual FLinearColor GetAssetColor() const override { return FLinearColor(FColor(156,232,188)); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UFlowGraphTemplate::StaticClass(); }
	
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const auto Categories =
			{ EAssetCategoryPaths::Gameplay / NSLOCTEXT("FlowGraphEditor", "FlowGraph Asset Category", "流图") };
		return Categories;
	}
	
	virtual FText GetObjectDisplayNameText(UObject* Object) const override { return FText::FromString(CastChecked<UFlowGraphTemplate>(Object)->GraphId.ToString()); }

	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;

	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};
