#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class FLOWGRAPHEDITOR_API FFlowGraphAssetEditorToolkit final : public FAssetEditorToolkit
{
public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	void InitializeAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InAssets);

private:

	TSharedPtr<FUICommandList> GraphEditorUICommands;

	inline static const FName& DetailViewPanelTabName {"DetailViewTab"};
	inline static const FName& GraphEditorTabName {"GraphEditorTab"};

	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<IDetailsView> DetailsView;

	TSharedRef<SDockTab> SpawnDetailViewTab(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnGraphEditorTab(const FSpawnTabArgs& SpawnTabArgs);
	

	void CreateGraphEditorUICommands();

	void DeleteSelectedNodes() const;

	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
};
