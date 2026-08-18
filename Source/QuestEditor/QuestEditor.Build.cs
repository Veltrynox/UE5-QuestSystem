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
        // Use explicit or shared Precompiled Headers (PCH) for faster compilation times
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // Public module dependencies required by header files in Public/
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",         // Core Unreal Engine data types and utilities
                "CoreUObject",  // UObject system, garbage collection, and reflection
                "Engine",       // Engine runtime functionality and asset base classes
                "Quest",        // Runtime Quest module containing UQuestGraph & UQuestNode
                "UnrealEd",     // Base Unreal Editor functionality (UFactory, Asset Editors)
                "AssetTools"    // AssetTools module for registering custom Asset Type Actions
            }
        );

        // Private module dependencies used exclusively inside Private/ implementation files
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",            // Core Slate UI framework used for editor windows & widgets
                "SlateCore",        // Core Slate rendering, styling, and widget primitives
                "GraphEditor",      // Visual Graph Editor Slate widgets (SGraphEditor)
                "KismetWidgets",    // Editor UI widgets for Blueprint/Graph tools
                "PropertyEditor",   // Details Panel customization and property views
                "ApplicationCore"   // Platform application windowing & input core
            }
        );
    }
}