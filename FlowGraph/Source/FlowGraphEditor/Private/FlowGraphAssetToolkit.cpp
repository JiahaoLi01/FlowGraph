#include "FlowGraphAssetToolkit.h"

#include "EdGraphUtilities.h"
#include "FlowGraph_Graph.h"
#include "FlowGraphTemplate.h"
#include "FlowGraph_Node.h"
#include "SNodePanel.h"
#include "Framework/Commands/GenericCommands.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

FName FFlowGraphAssetEditorToolkit::GetToolkitFName() const
{
	return {"FlowGraphEditorToolkit"};
}

FText FFlowGraphAssetEditorToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("FlowGraphEditor", "FlowGraphEditorToolkit", "流图编辑器");
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
		.SetDisplayName(NSLOCTEXT("FlowGraphEditor", "FlowGraphEditorToolkitDetailTab", "细节页签"))
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Toolbar.Details"));
	InTabManager->RegisterTabSpawner(GraphEditorTabName, FOnSpawnTab::CreateRaw(this, &FFlowGraphAssetEditorToolkit::SpawnGraphEditorTab))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetDisplayName(NSLOCTEXT("FlowGraphEditor", "FlowGraphEditorGraphEditorTab", "图编辑页签"))
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
	GraphAppearanceInfo.CornerText = NSLOCTEXT("FlowGraphEditor", "FlowGraphEditorCornerText", "流图");

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
	GraphEditorUICommands->MapAction(FGenericCommands::Get().Copy, FExecuteAction::CreateRaw(this, &FFlowGraphAssetEditorToolkit::CopySelectedNodes));
	GraphEditorUICommands->MapAction(FGenericCommands::Get().Cut, FExecuteAction::CreateRaw(this, &FFlowGraphAssetEditorToolkit::CutSelectedNodes));
	GraphEditorUICommands->MapAction(FGenericCommands::Get().Paste, FExecuteAction::CreateRaw(this, &FFlowGraphAssetEditorToolkit::PasteNodes));
}

void FFlowGraphAssetEditorToolkit::DeleteSelectedNodes()
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

void FFlowGraphAssetEditorToolkit::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	
	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if(UFlowGraph_Node* Node = Cast<UFlowGraph_Node>(*SelectedIter))
		{
			Node->PrepareForCopying();
		}
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

void FFlowGraphAssetEditorToolkit::CutSelectedNodes()
{

	CopySelectedNodes();
	// Cut should only delete nodes that can be duplicated
	DeleteSelectedDuplicatableNodes();
}

void FFlowGraphAssetEditorToolkit::DeleteSelectedDuplicatableNodes()
{
	// Cache off the old selection
	const FGraphPanelSelectionSet OldSelectedNodes = GraphEditor->GetSelectedNodes();

	// Clear the selection and only select the nodes that can be duplicated
	FGraphPanelSelectionSet RemainingNodes;
	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter); (Node != nullptr) && Node->CanDuplicateNode())
		{
			GraphEditor->SetNodeSelection(Node, true);
		}
		else
		{
			RemainingNodes.Add(Node);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	// Reselect whatever is left from the original selection after the deletion
	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(RemainingNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			GraphEditor->SetNodeSelection(Node, true);
		}
	}
}


void FFlowGraphAssetEditorToolkit::PasteNodes()
{
	const FVector2D Location{GraphEditor->GetPasteLocation()};
	// Undo/Redo support
	const FScopedTransaction Transaction( NSLOCTEXT("FlowGraphEditor", "Flow Graph Editor Paste Node", "黏贴节点") );
	GraphEditor->GetCurrentGraph()->Modify();

	// Clear the selection set (newly pasted stuff will be selected)
	GraphEditor->ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(GraphEditor->GetCurrentGraph(), TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f,0.0f);

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		const UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	if ( PastedNodes.Num() > 0 )
	{
		const float InvNumNodes = 1.0f/static_cast<float>(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;

		// Select the newly pasted stuff
		GraphEditor->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X ;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y ;

		Node->SnapToGrid(SNodePanel::GetSnapGridSize());

		// Give new node a different Guid from the old one
		Node->CreateNewGuid();
	}
	// Update UI
	GraphEditor->NotifyGraphChanged();

	GetEditingObject()->PostEditChange();
	GetEditingObject()->MarkPackageDirty();
}

void FFlowGraphAssetEditorToolkit::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection) const
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
