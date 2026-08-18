# Quest System & Visual Graph Editor for Unreal Engine 5

A C++ plugin for Unreal Engine 5 providing a visual graph editor and runtime subsystem for managing non-linear branching quests.

## Features

- **Custom Editor Window & Slate Canvas**: Integrated visual editor with graph navigation, node creation, wire connections, and a Details Panel inspector.
- **Node Types**:
  - **Start Node (`UEdGraphNode_QuestStart`)**: Output-only entry point node.
  - **Step Node (`UEdGraphNode_Quest`)**: Dual-pin node for active objectives.
  - **End Node (`UEdGraphNode_QuestEnd`)**: Input-only completion node.
  - **Comment Frame (`UEdGraphNode_Comment`)**: Resizable organization box (`C` key shortcut).
- **Multi-Root DAG Support**: Multiple independent quest branches executing concurrently.
- **Milestone Styling**: Optional `bIsMainQuest` flag for visual hierarchy (`[MAIN QUEST]` badge).
- **Activation Thresholds (`RequiredCompletionsCount`)**: Configurable parent completion requirement supporting AND/OR/Optional logic.
- **Passive Conditions (`UQuestCondition`)**: Instanced condition checks (World Tags, Node States, or custom Blueprint conditions) evaluated prior to step completion.
- **Runtime Manager (`UQuestManager`)**: World subsystem handling step transitions, object interactions, and world state flags.
- **Runtime JSON Serialization**: Save and restore active graph execution state to and from JSON files.

## Architecture

```
FQuestEditorModule
  └── FQuestGraphAssetEditor (Slate Asset Editor Toolkit)
        └── UEdGraph_Quest (Editor-only Visual Canvas)
              └── UQuestGraph (Runtime Data Asset)
                    └── UQuestManager (Runtime World Subsystem)
```

## Key Implementation Details

1. **Graph Serialization**: Visual canvas links (`LinkedTo`) dynamically reconstruct runtime `ChildNodes` and `RootNodes` arrays upon graph modification.
2. **Editor Data Stripping**: Visual graph properties are wrapped in `#if WITH_EDITORONLY_DATA` to ensure zero memory overhead in packaged builds.
3. **Transaction Support**: Full integration with Unreal Engine's `GUndo` transaction buffer for Undo/Redo (`Ctrl+Z` / `Ctrl+Y`).
4. **Polymorphic Conditions**: Utilizes `EditInlineNew` and `Instanced` properties to allow inline condition construction within the Details Panel.

## Directory Structure

```
Plugins/QuestSystem/
└── Source/
    ├── Quest/                          # Runtime Gameplay Module
    │   ├── QuestGraph.h / .cpp         # UQuestGraph Data Asset
    │   ├── QuestNode.h / .cpp          # UQuestNode Objective Data Object
    │   ├── QuestCondition.h / .cpp     # UQuestCondition Base & Subclasses
    │   └── QuestManager.h / .cpp       # UQuestManager World Subsystem
    │
    └── QuestEditor/                    # Editor Plugin Module
        ├── Public/
        │   ├── QuestGraphAssetEditor.h # FQuestGraphAssetEditor Slate Window Toolkit
        │   ├── EdGraph_Quest.h         # UEdGraph_Quest Visual Canvas
        │   ├── EdGraphSchema_Quest.h   # UEdGraphSchema_Quest Schema & Actions
        │   ├── EdGraphNode_Quest.h     # UEdGraphNode_Quest Base Visual Node
        │   ├── EdGraphNode_QuestStart.h# UEdGraphNode_QuestStart
        │   ├── EdGraphNode_QuestEnd.h  # UEdGraphNode_QuestEnd
        │   ├── QuestGraphFactory.h     # Asset Factory
        │   └── AssetTypeActions_...h   # Asset Type Actions
        └── Private/                    # C++ Implementations
```

## Basic Usage

```cpp
// Get Quest Manager Subsystem
UQuestManager* QuestManager = GetWorld()->GetSubsystem<UQuestManager>();

// Start Quest Graph
QuestManager->StartQuestGraph(MyQuestGraphAsset);

// Advance Step on Interaction
QuestManager->OnObjectInteracted(FName("TalkToBartender"));

// Save Progress to JSON
QuestManager->SaveQuestProgressToJson(TEXT("SaveSlot_01"));

// Load Progress from JSON
QuestManager->LoadQuestProgressFromJson(TEXT("SaveSlot_01"));
```
