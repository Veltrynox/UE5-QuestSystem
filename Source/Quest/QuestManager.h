// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestGraph.h"
#include "QuestNode.h"
#include "QuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalInteracted, FName, GoalID);

/**
 * UQuestManager
 * World subsystem managing active quest graph execution and step transitions at runtime.
 */
UCLASS()
class QUEST_API UQuestManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Delegate fired when a quest goal interaction takes place */
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnGoalInteracted OnGoalInteracted;

	/** Currently active Quest Graph asset being executed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	TObjectPtr<UQuestGraph> ActiveQuestGraph;

	/** List of quest nodes currently in progress */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
	TArray<TObjectPtr<UQuestNode>> ActiveNodes;

	/** Starts execution of the specified Quest Graph asset */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void StartQuestGraph(UQuestGraph* GraphToStart);

	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool AdvanceStep(FName StepID);

	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool AdvanceNode(UQuestNode* NodeToComplete);

	/** Handles player interaction with a world object matching ObjectTag */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool OnObjectInteracted(FName ObjectTag);

	/** Set of active world tags / flags (e.g. "IsEnemyOfCity", "HasCellarKey") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	TSet<FName> ActiveWorldTags;

	/** Adds a world tag to ActiveWorldTags set */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void AddWorldTag(FName Tag);

	/** Removes a world tag from ActiveWorldTags set */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void RemoveWorldTag(FName Tag);

	/** Checks if a world tag is currently active */
	UFUNCTION(BlueprintPure, Category = "Quests")
	bool HasWorldTag(FName Tag) const;

	/** Saves current quest graph execution state and active world tags to a JSON file */
	UFUNCTION(BlueprintCallable, Category = "Quests|Save")
	bool SaveQuestProgressToJson(const FString& SlotName = TEXT("QuestSave"));

	/** Loads quest graph execution state and active world tags from a JSON save file */
	UFUNCTION(BlueprintCallable, Category = "Quests|Save")
	bool LoadQuestProgressFromJson(const FString& SlotName = TEXT("QuestSave"));

	/** Deprecated goal completion delegate trigger */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void CompleteGoal(FName GoalID);
};
