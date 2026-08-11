#include "AssetTypeActions_QuestGraph.h"
#include "Quest/QuestGraph.h"
#include "QuestGraphAssetEditor.h"

UClass* FAssetTypeActions_QuestGraph::GetSupportedClass() const
{
	// Bind these asset actions specifically to the UQuestGraph asset class
	return UQuestGraph::StaticClass();
}

uint32 FAssetTypeActions_QuestGraph::GetCategories()
{
	// Place the asset under the standard "Gameplay" category in the Content Browser context menu
	return EAssetTypeCategories::Gameplay;
}

void FAssetTypeActions_QuestGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	if (InObjects.Num() > 0)
	{
		if (UQuestGraph* QuestGraph = Cast<UQuestGraph>(InObjects[0]))
		{
			TSharedRef<FQuestGraphAssetEditor> NewEditor = MakeShared<FQuestGraphAssetEditor>();
			NewEditor->InitQuestGraphAssetEditor(Mode, EditWithinLevelEditor, QuestGraph);
		}
	}
}