#include "QuestEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetTypeActions_QuestGraph.h"

#define LOCTEXT_NAMESPACE "FQuestEditorModule"

void FQuestEditorModule::StartupModule()
{
    // Retrieve the AssetTools module instance to register our custom asset type actions
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    // Create a new instance of our custom asset type action for UQuestGraph
    TSharedRef<IAssetTypeActions> Action = MakeShared<FAssetTypeActions_QuestGraph>();
    CreatedAssetTypeAction = Action;

    // Register the asset type action so Unreal Editor recognizes UQuestGraph in Content Browser
    AssetTools.RegisterAssetTypeActions(Action);
}

void FQuestEditorModule::ShutdownModule()
{
    // Check if AssetTools module is still loaded before attempting cleanup during editor exit
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        
        // Safely unregister our asset type action to prevent memory leaks and dangling callbacks
        if (CreatedAssetTypeAction.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
        }
    }
}

#undef LOCTEXT_NAMESPACE
    
// Expose the module entry point symbol (InitializeModule) so FModuleManager can load and instantiate FQuestEditorModule
IMPLEMENT_MODULE(FQuestEditorModule, QuestEditor)