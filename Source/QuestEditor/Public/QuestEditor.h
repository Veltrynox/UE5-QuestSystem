#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"

/**
 * FQuestEditorModule
 * Main editor module implementation interface for the QuestEditor module.
 * Responsible for initializing editor extensions, registering custom AssetTypeActions,
 * and cleaning up editor resources when the module is unloaded.
 */
class FQuestEditorModule : public IModuleInterface
{
public:
    /**
     * Called immediately after the module DLL is loaded into memory by FModuleManager during Editor startup.
     * Used to register custom asset actions, factory categories, and Slate UI extensions.
     */
    virtual void StartupModule() override;

    /**
     * Called before the module is unloaded from memory during Editor shutdown or module reload.
     * Used to safely unregister asset actions to prevent dangling pointers in IAssetTools.
     */
    virtual void ShutdownModule() override;

private:
    /** Reference to the registered AssetTypeActions instance for UQuestGraph assets */
    TSharedPtr<IAssetTypeActions> CreatedAssetTypeAction;
};


