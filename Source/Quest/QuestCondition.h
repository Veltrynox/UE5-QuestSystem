#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestNode.h"
#include "QuestCondition.generated.h"

class UQuestManager;

/**
 * UQuestCondition
 * Abstract base class for passive conditions checked before completing a quest step.
 * Inherit from this class in C++ or Blueprint to create custom quest conditions.
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, Blueprintable, BlueprintType)
class QUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Evaluates whether this condition is satisfied at runtime.
	 * @param Manager Pointer to the active UQuestManager subsystem.
	 * @param OwningNode Pointer to the UQuestNode evaluating this condition.
	 * @return True if the condition is satisfied, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Quest Condition")
	bool IsConditionMet(UQuestManager* Manager, UQuestNode* OwningNode) const;
	virtual bool IsConditionMet_Implementation(UQuestManager* Manager, UQuestNode* OwningNode) const;
};

/**
 * UQuestCondition_CheckNodeState
 * Condition that checks if another quest node in the graph has a specific required status.
 */
UCLASS(DisplayName = "Check Node Status")
class QUEST_API UQuestCondition_CheckNodeState : public UQuestCondition
{
	GENERATED_BODY()

public:
	/** StepID of the target quest node to evaluate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FName TargetStepID;

	/** Required status of the target quest node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	EQuestStatus RequiredStatus = EQuestStatus::Completed;

	/** If true, condition succeeds when status does NOT match RequiredStatus */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	bool bInvertCondition = false;

	virtual bool IsConditionMet_Implementation(UQuestManager* Manager, UQuestNode* OwningNode) const override;
};

/**
 * UQuestCondition_CheckWorldTag
 * Condition that checks if a specific world tag / flag is active in UQuestManager.
 */
UCLASS(DisplayName = "Check World Tag")
class QUEST_API UQuestCondition_CheckWorldTag : public UQuestCondition
{
	GENERATED_BODY()

public:
	/** Tag identifier to evaluate in UQuestManager's ActiveWorldTags set */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FName WorldTag;

	/** If true, condition succeeds when the WorldTag is NOT active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	bool bInvertCondition = false;

	virtual bool IsConditionMet_Implementation(UQuestManager* Manager, UQuestNode* OwningNode) const override;
};
