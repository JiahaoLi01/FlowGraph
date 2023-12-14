#include "AssetDefinition_FlowGraph.h"
#include "FlowGraphTemplate.h"
#include "FlowGraphAssetToolkit.h"
#include "Interfaces/IPluginManager.h"

EAssetCommandResult UAssetDefinition_FlowGraph::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	const EToolkitMode::Type ToolKitModeType = OpenArgs.GetToolkitMode();

	for (const FAssetData& Asset : OpenArgs.Assets)
	{
		if (UFlowGraphTemplate* FlowGraphAsset = Cast<UFlowGraphTemplate>(Asset.GetAsset()))
		{
			const TSharedRef<FFlowGraphAssetEditorToolkit> FlowGraphAssetEditorToolkit = MakeShareable(new FFlowGraphAssetEditorToolkit());
			FlowGraphAssetEditorToolkit->InitializeAssetEditor(ToolKitModeType, OpenArgs.ToolkitHost, FlowGraphAsset);
			return EAssetCommandResult::Handled;
		}
	}
	
	return EAssetCommandResult::Unhandled;
}

const FSlateBrush* UAssetDefinition_FlowGraph::GetThumbnailBrush(const FAssetData& InAssetData,
	const FName InClassName) const
{
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin("FlowGraph")->GetBaseDir();
	const static  FSlateDynamicImageBrush* Icon =
		new FSlateDynamicImageBrush(
			*(FPaths::Combine(PluginBaseDir) / "Resources" / "FlowGraphAssetIcon.png"),
			FVector2D(256, 256));
	return Icon;
}
