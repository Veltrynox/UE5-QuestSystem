using UnrealBuildTool;

/**
 * QuestEditor Module Build Rules
 * Defines module dependencies and build configuration for the Quest Editor extension.
 * This module is loaded exclusively in Unreal Editor builds (Type = "Editor").
 */
public class QuestEditor : ModuleRules
{
    public QuestEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Quest",
                "UnrealEd",
                "AssetTools"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "GraphEditor",
                "KismetWidgets",
                "PropertyEditor",
                "ApplicationCore"
            }
        );
    }
}
