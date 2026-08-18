#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * FAssetTypeActions_QuestGraph
 * Defines appearance, context menu placement, and double-click editor opening
 * for UQuestGraph assets in the Content Browser.
 */
class FAssetTypeActions_QuestGraph : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_QuestGraph", "Quest Graph"); }
	virtual FColor GetTypeColor() const override { return FColor::FromHex("#32A852"); }
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor) override;
};
