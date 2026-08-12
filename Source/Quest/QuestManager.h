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

	/** Advances and completes the active step matching StepID */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool AdvanceStep(FName StepID);

	/** Handles player interaction with a world object matching ObjectTag */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool OnObjectInteracted(FName ObjectTag);

	/** Deprecated goal completion delegate trigger */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void CompleteGoal(FName GoalID);
};
