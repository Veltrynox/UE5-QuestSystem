#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UQuestGraph;
class UEdGraph_Quest;

/**
 * FQuestGraphAssetEditor
 * Custom asset editor toolkit for editing UQuestGraph assets in a visual Slate Graph Editor window.
 */
class QUESTEDITOR_API FQuestGraphAssetEditor : public FAssetEditorToolkit
{
public:
	/** Initialize and open the standalone asset editor window for the given UQuestGraph */
	void InitQuestGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestGraph* InQuestGraph);

	// FAssetEditorToolkit interface overrides
	virtual FName GetToolkitFName() const override { return FName("QuestGraphAssetEditor"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("Quest Graph Editor")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("QuestGraphAssetEditor"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.2f, 0.65f, 0.35f, 1.0f); }

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:
	/** Target UQuestGraph asset object currently being edited */
	TObjectPtr<UQuestGraph> QuestGraphAsset;

	/** Internal visual UEdGraph container created for editing */
	TObjectPtr<UEdGraph_Quest> EdGraph;

	/** Tab spawner callback for graph view */
	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
	
	/** Widget displaying details and properties for the currently selected node */
	TSharedPtr<class IDetailsView> DetailsView;
	
	/** Tab spawner callback for details view panel */
	TSharedRef<SDockTab> SpawnTab_DetailsView(const FSpawnTabArgs& Args);
	
	/** Callback triggered when node selection changes on the graph canvas */
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	
	/** Callback triggered when any property is modified in the details view */
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	
	/** Callback triggered when text is committed on any graph node (e.g. comment frame box header) */
	void OnNodeTextCommitted(const FText& InText, ETextCommit::Type CommitInfo, UEdGraphNode* Node);
	
	/** Reference to the Slate graph editor widget */
	TSharedPtr<SGraphEditor> GraphWidget;

	/** Maps editor actions like Delete to internal command handlers */
	void CreateCommandList();

	/** Deletes all currently selected nodes from the graph canvas */
	void DeleteSelectedNodes();

	/** Returns true if one or more nodes are currently selected for deletion */
	bool CanDeleteNodes() const;
};
