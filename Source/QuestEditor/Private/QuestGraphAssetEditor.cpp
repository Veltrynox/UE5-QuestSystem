#include "QuestGraphAssetEditor.h"
#include "Quest/QuestGraph.h"
#include "EdGraph_Quest.h"
#include "EdGraphSchema_Quest.h"
#include "GraphEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EdGraphNode_Quest.h"
#include "Framework/Commands/GenericCommands.h"
#include "ScopedTransaction.h"

static const FName QuestGraphCanvasTabId(TEXT("QuestGraph_GraphCanvasTab"));
static const FName QuestGraphDetailsTabId(TEXT("QuestGraph_DetailsTab"));

void FQuestGraphAssetEditor::InitQuestGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestGraph* InQuestGraph)
{
	QuestGraphAsset = InQuestGraph;

	// Restore persistent EdGraph or instantiate a new canvas if none exists
	if (!QuestGraphAsset->EdGraph)
	{
		EdGraph = NewObject<UEdGraph_Quest>(QuestGraphAsset, TEXT("QuestGraphViewport"), RF_Transactional);
		EdGraph->Schema = UEdGraphSchema_Quest::StaticClass();
		QuestGraphAsset->EdGraph = EdGraph;
	}
	else
	{
		EdGraph = Cast<UEdGraph_Quest>(QuestGraphAsset->EdGraph);
	}

	// Register keyboard shortcuts and commands
	CreateCommandList();

	// Define editor window layout split between Graph Viewport and Details Panel
	const TSharedRef<FTabManager::FLayout> StandaloneLayout = FTabManager::NewLayout("Standalone_QuestGraphEditor_Layout_v2")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.75f)
					->AddTab(QuestGraphCanvasTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.25f)
					->AddTab(QuestGraphDetailsTabId, ETabState::OpenedTab)
				)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, FName("QuestGraphEditorApp"), StandaloneLayout, true, true, InQuestGraph);
}

void FQuestGraphAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(QuestGraphCanvasTabId, FOnSpawnTab::CreateSP(this, &FQuestGraphAssetEditor::SpawnTab_GraphCanvas))
		.SetDisplayName(FText::FromString(TEXT("Graph Viewport")))
		.SetGroup(AssetEditorTabsCategory.ToSharedRef());
	
	InTabManager->RegisterTabSpawner(QuestGraphDetailsTabId, FOnSpawnTab::CreateSP(this, &FQuestGraphAssetEditor::SpawnTab_DetailsView))
		.SetDisplayName(FText::FromString(TEXT("Details")))
		.SetGroup(AssetEditorTabsCategory.ToSharedRef());
}

void FQuestGraphAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(QuestGraphCanvasTabId);
	InTabManager->UnregisterTabSpawner(QuestGraphDetailsTabId);
}

TSharedRef<SDockTab> FQuestGraphAssetEditor::SpawnTab_GraphCanvas(const FSpawnTabArgs& Args)
{
	SGraphEditor::FGraphEditorEvents Events;
	Events.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FQuestGraphAssetEditor::OnSelectedNodesChanged);
	Events.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FQuestGraphAssetEditor::OnNodeTextCommitted);
	
	SAssignNew(GraphWidget, SGraphEditor)
		.AdditionalCommands(GetToolkitCommands())
		.IsEditable(true)
		.GraphToEdit(EdGraph)
		.GraphEvents(Events);

	return SNew(SDockTab)
		.Label(FText::FromString(TEXT("Quest Graph Canvas")))
		[
			GraphWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FQuestGraphAssetEditor::SpawnTab_DetailsView(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->OnFinishedChangingProperties().AddSP(this, &FQuestGraphAssetEditor::OnPropertyChanged);

	return SNew(SDockTab)
		.Label(FText::FromString(TEXT("Details")))
		[
			DetailsView.ToSharedRef()
		];
}

void FQuestGraphAssetEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	if (!DetailsView.IsValid()) return;

	if (NewSelection.Num() > 0)
	{
		for (UObject* SelectedObject : NewSelection)
		{
			if (UEdGraphNode_Quest* SelectedEdNode = Cast<UEdGraphNode_Quest>(SelectedObject))
			{
				if (SelectedEdNode->QuestNode)
				{
					DetailsView->SetObject(SelectedEdNode->QuestNode);
					return;
				}
			}
		}
	}

	DetailsView->SetObject(nullptr);
}

void FQuestGraphAssetEditor::OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EdGraph)
	{
		EdGraph->NotifyGraphChanged();
	}
}

void FQuestGraphAssetEditor::OnNodeTextCommitted(const FText& InText, ETextCommit::Type CommitInfo, UEdGraphNode* Node)
{
	if (Node)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("QuestEditor", "EditNodeComment", "Edit Node Comment"));
		Node->Modify();
		Node->OnUpdateCommentText(InText.ToString());

		if (EdGraph)
		{
			EdGraph->NotifyGraphChanged();
		}
	}
}

void FQuestGraphAssetEditor::CreateCommandList()
{
	ToolkitCommands->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateSP(this, &FQuestGraphAssetEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FQuestGraphAssetEditor::CanDeleteNodes)
	);
}

void FQuestGraphAssetEditor::DeleteSelectedNodes()
{
	if (!GraphWidget.IsValid()) return;

	const FGraphPanelSelectionSet SelectedNodes = GraphWidget->GetSelectedNodes();
	if (SelectedNodes.Num() == 0) return;

	const FScopedTransaction Transaction(NSLOCTEXT("QuestEditor", "DeleteSelectedNodes", "Delete Selected Nodes"));
	EdGraph->Modify();

	for (UObject* NodeObj : SelectedNodes)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(NodeObj);
		if (Node && Node->CanUserDeleteNode())
		{
			Node->Modify();
			Node->DestroyNode();
		}
	}

	EdGraph->NotifyGraphChanged();
}

bool FQuestGraphAssetEditor::CanDeleteNodes() const
{
	if (GraphWidget.IsValid())
	{
		return GraphWidget->GetSelectedNodes().Num() > 0;
	}
	return false;
}
