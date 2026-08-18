#include "QuestManager.h"
#include "QuestCondition.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("=== QuestManager: Initialized successfully ==="));
}

void UQuestManager::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("=== QuestManager: Deinitialized ==="));
	Super::Deinitialize();
}

void UQuestManager::AddWorldTag(FName Tag)
{
	if (!Tag.IsNone())
	{
		ActiveWorldTags.Add(Tag);
		UE_LOG(LogTemp, Warning, TEXT("QuestManager: World Tag Added -> '%s'"), *Tag.ToString());
	}
}

void UQuestManager::RemoveWorldTag(FName Tag)
{
	if (!Tag.IsNone())
	{
		ActiveWorldTags.Remove(Tag);
		UE_LOG(LogTemp, Warning, TEXT("QuestManager: World Tag Removed -> '%s'"), *Tag.ToString());
	}
}

bool UQuestManager::HasWorldTag(FName Tag) const
{
	return !Tag.IsNone() && ActiveWorldTags.Contains(Tag);
}

bool UQuestManager::SaveQuestProgressToJson(const FString& SlotName)
{
	if (!ActiveQuestGraph)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestManager: Cannot save JSON — ActiveQuestGraph is nullptr!"));
		return false;
	}

	TSharedRef<FJsonObject> RootObject = MakeShared<FJsonObject>();

	// 1. Save Active Quest Graph Asset Path
	RootObject->SetStringField(TEXT("QuestGraphAsset"), ActiveQuestGraph->GetPathName());

	// 2. Save Active World Tags
	TArray<TSharedPtr<FJsonValue>> WorldTagsArray;
	for (const FName& Tag : ActiveWorldTags)
	{
		WorldTagsArray.Add(MakeShared<FJsonValueString>(Tag.ToString()));
	}
	RootObject->SetArrayField(TEXT("WorldTags"), WorldTagsArray);

	// 3. Save Node Progress States
	TArray<TSharedPtr<FJsonValue>> NodesArray;
	for (UQuestNode* Node : ActiveQuestGraph->AllNodes)
	{
		if (Node)
		{
			TSharedRef<FJsonObject> NodeObj = MakeShared<FJsonObject>();
			NodeObj->SetStringField(TEXT("StepID"), Node->StepID.ToString());
			NodeObj->SetNumberField(TEXT("Status"), static_cast<int32>(Node->Status));
			NodeObj->SetNumberField(TEXT("CompletedParents"), Node->CurrentCompletedParentsCount);

			NodesArray.Add(MakeShared<FJsonValueObject>(NodeObj));
		}
	}
	RootObject->SetArrayField(TEXT("Nodes"), NodesArray);

	// Write JSON string to project Saved/SaveGames folder
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (FJsonSerializer::Serialize(RootObject, Writer))
	{
		FString SaveFilePath = FPaths::ProjectSavedDir() / TEXT("SaveGames") / (SlotName + TEXT(".json"));
		if (FFileHelper::SaveStringToFile(JsonString, *SaveFilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestManager: Quest Progress SAVED to '%s'"), *SaveFilePath);

			if (GEngine)
			{
				FString Msg = FString::Printf(TEXT("💾 QUEST PROGRESS SAVED: %s.json"), *SlotName);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, Msg, true, FVector2D(1.8f, 1.8f));
			}

			return true;
		}
	}

	return false;
}

bool UQuestManager::LoadQuestProgressFromJson(const FString& SlotName)
{
	FString SaveFilePath = FPaths::ProjectSavedDir() / TEXT("SaveGames") / (SlotName + TEXT(".json"));
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *SaveFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestManager: Save file '%s' not found!"), *SaveFilePath);
		return false;
	}

	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("QuestManager: Failed to parse JSON save file!"));
		return false;
	}

	// 1. Load Quest Graph Asset if needed
	FString GraphAssetPath = RootObject->GetStringField(TEXT("QuestGraphAsset"));
	if (!GraphAssetPath.IsEmpty())
	{
		UQuestGraph* LoadedGraph = LoadObject<UQuestGraph>(nullptr, *GraphAssetPath);
		if (LoadedGraph)
		{
			ActiveQuestGraph = LoadedGraph;
		}
	}

	if (!ActiveQuestGraph)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestManager: Cannot restore progress — QuestGraph is missing!"));
		return false;
	}

	ActiveNodes.Empty();
	ActiveWorldTags.Empty();

	// 2. Restore Active World Tags
	const TArray<TSharedPtr<FJsonValue>>* WorldTagsArray;
	if (RootObject->TryGetArrayField(TEXT("WorldTags"), WorldTagsArray))
	{
		for (const auto& TagVal : *WorldTagsArray)
		{
			ActiveWorldTags.Add(FName(*TagVal->AsString()));
		}
	}

	// 3. Restore Node Progress States
	const TArray<TSharedPtr<FJsonValue>>* NodesArray;
	if (RootObject->TryGetArrayField(TEXT("Nodes"), NodesArray))
	{
		for (const auto& NodeVal : *NodesArray)
		{
			TSharedPtr<FJsonObject> NodeObj = NodeVal->AsObject();
			if (NodeObj.IsValid())
			{
				FName StepID = FName(*NodeObj->GetStringField(TEXT("StepID")));
				int32 StatusInt = NodeObj->GetIntegerField(TEXT("Status"));
				int32 CompletedParents = NodeObj->GetIntegerField(TEXT("CompletedParents"));

				for (UQuestNode* Node : ActiveQuestGraph->AllNodes)
				{
					if (Node && Node->StepID == StepID)
					{
						Node->Status = static_cast<EQuestStatus>(StatusInt);
						Node->CurrentCompletedParentsCount = CompletedParents;

						if (Node->Status == EQuestStatus::InProgress)
						{
							ActiveNodes.AddUnique(Node);
						}
						break;
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("QuestManager: Quest Progress LOADED from '%s' (Active Nodes: %d)"), 
		*SaveFilePath, ActiveNodes.Num());

	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("📂 QUEST PROGRESS LOADED: %s.json"), *SlotName);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, Msg, true, FVector2D(1.8f, 1.8f));
	}

	return true;
}

bool UQuestManager::OnObjectInteracted(FName ObjectTag)
{
	if (ObjectTag.IsNone()) return false;

	UE_LOG(LogTemp, Warning, TEXT("QuestManager: Player interacted with object '%s'"), *ObjectTag.ToString());

	// Find any currently active node expecting interaction with ObjectTag
	for (UQuestNode* ActiveNode : ActiveNodes)
	{
		if (ActiveNode && ActiveNode->TargetTag == ObjectTag)
		{
			// Evaluate all passive conditions attached to this active step
			for (UQuestCondition* Condition : ActiveNode->Conditions)
			{
				if (Condition && !Condition->IsConditionMet(this, ActiveNode))
				{
					UE_LOG(LogTemp, Warning, TEXT("QuestManager: Cannot complete step '%s' — Condition Failed!"), 
						*ActiveNode->StepID.ToString());

					if (GEngine)
					{
						FString Msg = FString::Printf(TEXT("⛔ INTERACTION BLOCKED: %s (Condition Failed)"), *ActiveNode->StepID.ToString());
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Msg, true, FVector2D(1.8f, 1.8f));
					}

					return false;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("QuestManager: Found matching active quest step '%s' for object '%s'!"), 
				*ActiveNode->StepID.ToString(), *ObjectTag.ToString());

			return AdvanceNode(ActiveNode);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("QuestManager: No active quest step requires interaction with '%s'"), *ObjectTag.ToString());
	return false;
}

void UQuestManager::CompleteGoal(FName GoalID)
{
	UE_LOG(LogTemp, Warning, TEXT("Goal completed: %s"), *GoalID.ToString());
}

void UQuestManager::StartQuestGraph(UQuestGraph* GraphToStart)
{
	if (!GraphToStart)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestManager: Cannot start nullptr QuestGraph!"));
		return;
	}

	ActiveQuestGraph = GraphToStart;
	ActiveNodes.Empty();

	// Reset runtime completion counters for all nodes in the graph
	for (UQuestNode* Node : ActiveQuestGraph->AllNodes)
	{
		if (Node)
		{
			Node->Status = EQuestStatus::NotStarted;
			Node->CurrentCompletedParentsCount = 0;
		}
	}

	// Initialize all root nodes in the graph as active in-progress steps
	for (UQuestNode* RootNode : ActiveQuestGraph->RootNodes)
	{
		if (RootNode)
		{
			RootNode->Status = EQuestStatus::InProgress;
			ActiveNodes.Add(RootNode);
			UE_LOG(LogTemp, Warning, TEXT("=== QUEST STARTED! Active Step: %s ==="), *RootNode->StepID.ToString());
			
			if (GEngine)
			{
				FString Msg = FString::Printf(TEXT("=== QUEST STARTED: %s ==="), *RootNode->StepID.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Msg, true, FVector2D(1.8f, 1.8f));
			}
		}
	}
}

bool UQuestManager::AdvanceNode(UQuestNode* NodeToComplete)
{
	if (!NodeToComplete) return false;

	int32 NodeIndex = ActiveNodes.IndexOfByKey(NodeToComplete);
	if (NodeIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestManager: Node %s is NOT currently active!"), *NodeToComplete->StepID.ToString());
		return false;
	}

	NodeToComplete->Status = EQuestStatus::Completed;
	ActiveNodes.RemoveAt(NodeIndex);
	UE_LOG(LogTemp, Warning, TEXT("=== STEP COMPLETED: %s (Child Nodes Count: %d) ==="), *NodeToComplete->StepID.ToString(), NodeToComplete->ChildNodes.Num());

	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("STEP COMPLETED: %s"), *NodeToComplete->StepID.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Msg, true, FVector2D(1.8f, 1.8f));
	}
	
	// Evaluate activation threshold for each child node
	for (UQuestNode* ChildNode : NodeToComplete->ChildNodes)
	{
		if (ChildNode && ChildNode->Status != EQuestStatus::Completed)
		{
			ChildNode->CurrentCompletedParentsCount++;

			// Determine required completions threshold (default to 1 if <= 0)
			int32 Threshold = (ChildNode->RequiredCompletionsCount > 0) ? ChildNode->RequiredCompletionsCount : 1;

			if (ChildNode->CurrentCompletedParentsCount >= Threshold)
			{
				ChildNode->Status = EQuestStatus::InProgress;
				ActiveNodes.AddUnique(ChildNode);
				UE_LOG(LogTemp, Warning, TEXT("--> NEXT ACTIVE STEP ACTIVATED: %s (Completed Parents: %d/%d)"), 
					*ChildNode->StepID.ToString(), ChildNode->CurrentCompletedParentsCount, Threshold);

				if (GEngine)
				{
					FString Msg = FString::Printf(TEXT("NEW STEP ACTIVE: %s (%d/%d)"), 
						*ChildNode->StepID.ToString(), ChildNode->CurrentCompletedParentsCount, Threshold);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Msg, true, FVector2D(1.8f, 1.8f));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("--> Step %s pending requirements (%d/%d parents completed)"), 
					*ChildNode->StepID.ToString(), ChildNode->CurrentCompletedParentsCount, Threshold);
			}
		}
	}

	if (ActiveNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("=== ALL QUEST STEPS COMPLETED! QUEST FINISHED! ==="));
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Yellow, TEXT("🎉 ALL QUEST STEPS COMPLETED!"), true, FVector2D(2.0f, 2.0f));
		}
	}

	return true;
}

bool UQuestManager::AdvanceStep(FName StepID)
{
	if (StepID.IsNone()) return false;

	for (UQuestNode* ActiveNode : ActiveNodes)
	{
		if (ActiveNode && ActiveNode->StepID == StepID)
		{
			return AdvanceNode(ActiveNode);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("QuestManager: Step %s is NOT currently active!"), *StepID.ToString());
	return false;
}
