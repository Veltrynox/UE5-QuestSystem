#include "QuestManager.h"

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

bool UQuestManager::OnObjectInteracted(FName ObjectTag)
{
	if (ObjectTag.IsNone()) return false;

	UE_LOG(LogTemp, Warning, TEXT("QuestManager: Player interacted with object '%s'"), *ObjectTag.ToString());

	// Find any currently active node expecting interaction with ObjectTag
	for (UQuestNode* ActiveNode : ActiveNodes)
	{
		if (ActiveNode && ActiveNode->TargetTag == ObjectTag)
		{
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
	UE_LOG(LogTemp, Warning, TEXT("=== STEP COMPLETED: %s ==="), *NodeToComplete->StepID.ToString());

	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("STEP COMPLETED: %s"), *NodeToComplete->StepID.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Msg, true, FVector2D(1.8f, 1.8f));
	}
	
	for (UQuestNode* ChildNode : NodeToComplete->ChildNodes)
	{
		if (ChildNode)
		{
			ChildNode->Status = EQuestStatus::InProgress;
			ActiveNodes.AddUnique(ChildNode);
			UE_LOG(LogTemp, Warning, TEXT("--> NEXT ACTIVE STEP: %s"), *ChildNode->StepID.ToString());
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