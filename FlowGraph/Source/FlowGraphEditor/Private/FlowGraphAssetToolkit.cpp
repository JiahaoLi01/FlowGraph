#include "FlowGraphAssetToolkit.h"
#include "FlowGraph_Graph.h"
#include "FlowGraphTemplate.h"
#include "Framework/Commands/GenericCommands.h"

FName FFlowGraphAssetEditorToolkit::GetToolkitFName() const
{
	return {"FlowGraphEditorToolkit"};
}

FText FFlowGraphAssetEditorToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("DarkMountainEditor", "FlowGraphEditorToolkit", "流图编辑器");
}

FString FFlowGraphAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return {"FlowGraphEditorToolkit"};
}

FLinearColor FFlowGraphAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Green;
}

void FFlowGraphAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(DetailViewPanelTabName, FOnSpawnTab::CreateRaw(this, &FFlowGraphAssetEditorToolkit::SpawnDetailViewTab))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetDisplayName(NSLOCTEXT("DarkMountainEditor", "FlowGraphEditorToolkitDetailTab", "细节页签"))
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Toolbar.Details"));
	InTabManager->RegisterTabSpawner(GraphEditorTabName, FOnSpawnTab::CreateRaw(this, &FFlowGraphAssetEditorToolkit::SpawnGraphEditorTab))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetDisplayName(NSLOCTEXT("DarkMountainEditor", "FlowGraphEditorGraphEditorTab", "图编辑页签"))
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("FlowGraphEditorToolkitMenuCategory"));
}

void FFlowGraphAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(DetailViewPanelTabName);
	InTabManager->UnregisterTabSpawner(GraphEditorTabName);
}

void FFlowGraphAssetEditorToolkit::InitializeAssetEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InAssets)
{
	const TSharedRef<FTabManager::FLayout> StandaloneOurAssetEditor = FTabManager::NewLayout("FlowGraphEditorLayout")->AddArea
			(
				FTabManager::NewPrimaryArea()->SetOrientation(EOrientation::Orient_Horizontal)
				->Split(FTabManager::NewStack()->AddTab(FName("GraphEditorTab"), ETabState::OpenedTab))
				->Split(FTabManager::NewStack()->AddTab(FName("DetailViewTab"), ETabState::OpenedTab))
			);
	InitAssetEditor(Mode, InitToolkitHost, FName("FlowGraphEditorToolkit"), StandaloneOurAssetEditor, true, true, InAssets);
	RegenerateMenusAndToolbars();
}

TSharedRef<SDockTab> FFlowGraphAssetEditorToolkit::SpawnDetailViewTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs;
	const TSharedRef<IDetailsView> AssetPropertyView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	DetailsView = AssetPropertyView;

	// Note that the outer of the graph is the actual flow graph object.
	AssetPropertyView->SetObject(GetEditingObject());
	return SNew(SDockTab)
	[
		AssetPropertyView
	];
}

TSharedRef<SDockTab> FFlowGraphAssetEditorToolkit::SpawnGraphEditorTab(const FSpawnTabArgs& SpawnTabArgs)
{
	CreateGraphEditorUICommands();
	
	const UFlowGraphTemplate* EditingFlowGraph = Cast<UFlowGraphTemplate>(GetEditingObject());
	if (EditingFlowGraph == nullptr)
	{
		return SNew(SDockTab);
	}

	FGraphAppearanceInfo GraphAppearanceInfo;
	GraphAppearanceInfo.CornerText = NSLOCTEXT("DarkMountainEditor", "FlowGraphEditorCornerText", "流图");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FFlowGraphAssetEditorToolkit::OnSelectedNodesChanged);

	GraphEditor = SNew(SGraphEditor)
					.GraphToEdit(EditingFlowGraph->Graph)
					.AdditionalCommands(GraphEditorUICommands)
					.Appearance(GraphAppearanceInfo)
					.GraphEvents(InEvents);

	return SNew(SDockTab)
	[
		GraphEditor.ToSharedRef()
	];
}

void FFlowGraphAssetEditorToolkit::CreateGraphEditorUICommands()
{
	GraphEditorUICommands = MakeShareable(new FUICommandList);
	GraphEditorUICommands->MapAction(FGenericCommands::Get().Delete,FExecuteAction::CreateRaw(this, &FFlowGraphAssetEditorToolkit::DeleteSelectedNodes));
}

void FFlowGraphAssetEditorToolkit::DeleteSelectedNodes() const
{
	if (!GraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	GraphEditor->GetCurrentGraph()->Modify();
	
	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIterator(SelectedNodes); NodeIterator; ++NodeIterator)
	{
		if (UEdGraphNode* GraphNodeToDelete = Cast<UEdGraphNode>(*NodeIterator))
		{
			if (GraphNodeToDelete->CanUserDeleteNode())
			{
				GraphNodeToDelete->Modify();
				GraphNodeToDelete->DestroyNode();
			}
		}
	}
}

void FFlowGraphAssetEditorToolkit::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}
	if (Selection.Num() == 0) 
	{
		DetailsView->SetObject(GetEditingObject());
	}
	else
	{
		DetailsView->SetObjects(Selection);
	}
}
