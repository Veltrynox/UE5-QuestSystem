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
	void InitQuestGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UQuestGraph* InQuestGraph);

	virtual FName GetToolkitFName() const override { return FName("QuestGraphAssetEditor"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("Quest Graph Editor")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("QuestGraphAssetEditor"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.2f, 0.65f, 0.35f, 1.0f); }

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:
	TObjectPtr<UQuestGraph> QuestGraphAsset;
	TObjectPtr<UEdGraph_Quest> EdGraph;

	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedRef<SDockTab> SpawnTab_DetailsView(const FSpawnTabArgs& Args);
	
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnNodeTextCommitted(const FText& InText, ETextCommit::Type CommitInfo, UEdGraphNode* Node);
	
	TSharedPtr<SGraphEditor> GraphWidget;

	void CreateCommandList();
	void DeleteSelectedNodes();
	bool CanDeleteNodes() const;
};
