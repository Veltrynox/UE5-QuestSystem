#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"

/**
 * FQuestEditorModule
 * Main editor module implementation interface for the QuestEditor module.
 */
class FQuestEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TSharedPtr<IAssetTypeActions> CreatedAssetTypeAction;
};
