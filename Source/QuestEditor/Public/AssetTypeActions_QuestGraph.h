#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * FAssetTypeActions_QuestGraph
 * Defines the appearance, context menu placement, color coding, and double-click actions
 * for UQuestGraph assets in the Unreal Editor Content Browser.
 */
class FAssetTypeActions_QuestGraph : public FAssetTypeActions_Base
{
public:
	/** Returns the localized display name shown in menus and tooltips */
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_QuestGraph", "Quest Graph"); }
    
	/** Returns the color tint applied to the asset thumbnail border in the Content Browser */
	virtual FColor GetTypeColor() const override { return FColor::FromHex("#32A852"); } // Emerald Green
    
	/** Returns the C++ class associated with these asset type actions */
	virtual UClass* GetSupportedClass() const override;
    
	/** Returns the category bitmask placing this asset into Content Browser context menus */
	virtual uint32 GetCategories() override;

	/** Called when the user double-clicks the asset thumbnail to open its custom editor window */
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor) override;
};