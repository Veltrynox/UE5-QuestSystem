#include "QuestEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetTypeActions_QuestGraph.h"

#define LOCTEXT_NAMESPACE "FQuestEditorModule"

void FQuestEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    TSharedRef<IAssetTypeActions> Action = MakeShared<FAssetTypeActions_QuestGraph>();
    CreatedAssetTypeAction = Action;

    AssetTools.RegisterAssetTypeActions(Action);
}

void FQuestEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        
        if (CreatedAssetTypeAction.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
        }
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FQuestEditorModule, QuestEditor)
