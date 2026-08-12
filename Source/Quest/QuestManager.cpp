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

			return AdvanceStep(ActiveNode->StepID);
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
		}
	}
}

bool UQuestManager::AdvanceStep(FName StepID)
{
	if (StepID.IsNone()) return false;

	UQuestNode* NodeToComplete = nullptr;
	int32 NodeIndex = INDEX_NONE;

	// Search active nodes list for matching StepID
	for (int32 i = 0; i < ActiveNodes.Num(); ++i)
	{
		if (ActiveNodes[i] && ActiveNodes[i]->StepID == StepID)
		{
			NodeToComplete = ActiveNodes[i];
			NodeIndex = i;
			break;
		}
	}

	if (!NodeToComplete)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestManager: Step %s is NOT currently active!"), *StepID.ToString());
		return false;
	}

	// Complete current step and remove from active list
	NodeToComplete->Status = EQuestStatus::Completed;
	ActiveNodes.RemoveAt(NodeIndex);
	UE_LOG(LogTemp, Warning, TEXT("=== STEP COMPLETED: %s ==="), *StepID.ToString());

	// Activate all subsequent child nodes
	for (UQuestNode* ChildNode : NodeToComplete->ChildNodes)
	{
		if (ChildNode)
		{
			ChildNode->Status = EQuestStatus::InProgress;
			ActiveNodes.AddUnique(ChildNode);
			UE_LOG(LogTemp, Warning, TEXT("--> NEXT ACTIVE STEP: %s"), *ChildNode->StepID.ToString());
		}
	}

	// Check if all steps in the graph have been completed
	if (ActiveNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("=== ALL QUEST STEPS COMPLETED! QUEST FINISHED! ==="));
	}

	return true;
}