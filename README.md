# Quest System & Visual Graph Editor for Unreal Engine 5

A production-grade, modular **Quest System and Custom Slate Visual Graph Editor Plugin** built for Unreal Engine 5 in C++. Designed for RPGs and narrative games requiring non-linear branching quests, multi-root DAG graphs, passive condition validation, and runtime JSON save/load capabilities.

---

## 🌟 Key Features

* **Custom Editor Window & Slate Canvas**: Integrated visual editor with graph navigation, node creation, drag-and-drop wire links, and real-time Details Panel inspector.
* **Specialized Visual Nodes**:
  * 🌟 **Start Node (`UEdGraphNode_QuestStart`)**: Golden header, output-only pin for quest entry points.
  * 🟢 **Step Node (`UEdGraphNode_Quest`)**: Dual-pin (Input/Output) node for active objectives.
  * 🔴 **End Node (`UEdGraphNode_QuestEnd`)**: Crimson header, input-only pin for quest completion milestones.
  * 💬 **Comment Frame (`UEdGraphNode_Comment`)**: Resizable organization boxes (`C` key shortcut) for quest chapters.
* **Multi-Root DAG Support**: Supports multiple independent starting quest branches executing simultaneously.
* **Milestone Styling**: Toggleable `bIsMainQuest` flag rendering royal purple headers and `⭐ [MAIN QUEST]` badges.
* **Activation Thresholds (`RequiredCompletionsCount`)**: Configurable parent completion threshold supporting AND/OR/Optional branching logic (e.g., require 3 out of 3 or 2 out of 5 parent tasks).
* **Passive Condition System (`UQuestCondition`)**: Modular, instanced condition evaluation (World Tag checks, Node State checks, or custom Blueprint conditions) executed before step completion.
* **Runtime World Subsystem (`UQuestManager`)**: Automatically managed `UWorldSubsystem` handling step transitions, player interactions (`OnObjectInteracted`), and debug messages.
* **Runtime JSON Save / Load**: Saves and restores active graph execution states, node statuses, parent counters, and world tags to/from `Saved/SaveGames/*.json`.

---

## 📐 Architecture & Data Flow

```
┌────────────────────────────────────────────────────────────────────────┐
│                        FQuestEditorModule                              │
│         (Module Entry Point: Registers Asset Type Actions)             │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Double-Click Asset
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                     FQuestGraphAssetEditor                             │
│   (Custom Slate Asset Editor Window: Docking Tabs, Details Panel)      │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Edits Visual Canvas
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                        UEdGraph_Quest                                  │
│   (Editor-only Visual Canvas: UEdGraphNode_Quest, X/Y Coordinates)     │
│   Calls RebuildQuestGraph() on Graph Changed                           │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Serializes Data (Cooker Stripped)
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                         UQuestGraph                                    │
│   (Runtime Data Asset: AllNodes, RootNodes)                            │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Executed at Runtime by
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                        UQuestManager                                   │
│   (UWorldSubsystem: Step Execution, Passive Conditions, JSON S/L)      │
└────────────────────────────────────────────────────────────────────────┘
```

---

## ⚙️ Key Algorithms & Design Patterns

1. **Topological Graph Synchronization (`RebuildQuestGraph`)**:
   * Scans visual node connections (`LinkedTo`), computes `bIsRootNode` status dynamically, and rebuilds runtime `ChildNodes` and `RootNodes` arrays.
2. **Editor Data Stripping (`WITH_EDITORONLY_DATA`)**:
   * Uses `#if WITH_EDITORONLY_DATA` for canvas pointers (`EdGraph`), node coordinates, and editor color tints. The Unreal Asset Cooker strips all visual nodes in Shipping builds, leaving zero memory overhead.
3. **Transaction & Undo/Redo (`RF_Transactional` / `FScopedTransaction`)**:
   * All node creation, deletion, and property mutations integrate with Unreal's `GUndo` transaction buffer (`Ctrl+Z` / `Ctrl+Y`).
4. **Instanced Polymorphic Conditions**:
   * Uses `EditInlineNew` and `Instanced` properties for `UQuestCondition` array, enabling designers to construct inline polymorphic condition chains directly inside the node Details Panel.

---

## 📁 Directory Structure

```
Source/
├── Quest/                                # Runtime Gameplay Module
│   ├── QuestGraph.h / .cpp               # UQuestGraph Data Asset
│   ├── QuestNode.h / .cpp                # UQuestNode Objective Data Object
│   ├── QuestCondition.h / .cpp           # UQuestCondition Base & Subclasses
│   └── QuestManager.h / .cpp             # UQuestManager World Subsystem
│
└── QuestEditor/                          # Editor Plugin Module
    ├── Public/
    │   ├── QuestGraphAssetEditor.h       # FQuestGraphAssetEditor Slate Window Toolkit
    │   ├── EdGraph_Quest.h               # UEdGraph_Quest Visual Canvas
    │   ├── EdGraphSchema_Quest.h         # UEdGraphSchema_Quest Schema & Actions
    │   ├── EdGraphNode_Quest.h           # UEdGraphNode_Quest Base Visual Node
    │   ├── EdGraphNode_QuestStart.h      # UEdGraphNode_QuestStart (Gold)
    │   ├── EdGraphNode_QuestEnd.h        # UEdGraphNode_QuestEnd (Red)
    │   ├── QuestGraphFactory.h           # Asset Factory for Content Browser
    │   └── AssetTypeActions_QuestGraph.h # Asset Type Actions & Double-click
    └── Private/                          # C++ Implementations
```

---

## 🎮 Runtime Usage Example

```cpp
// 1. Get Quest Manager Subsystem
UQuestManager* QuestManager = GetWorld()->GetSubsystem<UQuestManager>();

// 2. Start a Quest Graph
QuestManager->StartQuestGraph(MyQuestGraphAsset);

// 3. Handle Player Interaction
QuestManager->OnObjectInteracted(FName("TalkToBartender"));

// 4. Save Progress to JSON
QuestManager->SaveQuestProgressToJson(TEXT("SaveSlot_01"));

// 5. Load Progress from JSON
QuestManager->LoadQuestProgressFromJson(TEXT("SaveSlot_01"));
```
