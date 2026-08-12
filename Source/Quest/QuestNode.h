#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestNode.generated.h"

/**
 * Represents the current execution status of an individual quest step.
 */
UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	NotStarted	UMETA(DisplayName = "Not Started"),
	InProgress	UMETA(DisplayName = "In Progress"),
	Completed	UMETA(DisplayName = "Completed")
};

/**
 * UQuestNode
 * Runtime data object representing a single step/objective within a Quest Graph.
 * 
 * UQuestNode exists at runtime in the game to track quest logic, objectives, 
 * current status, and connections to subsequent child quest steps.
 */
UCLASS(BlueprintType, Blueprintable)
class QUEST_API UQuestNode : public UObject
{
	GENERATED_BODY()

public:
	/** Unique identifier for this quest step (e.g. "Step_KillWolves") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Node")
	FName StepID;

	/** Player-facing description or task objective displayed in the Quest Log UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Node")
	FText Description;

	/** Current progress status of this specific quest step */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Node")
	EQuestStatus Status = EQuestStatus::NotStarted;
	
	/** Direct links to subsequent quest steps triggered after completing this step */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Node")
	TArray<TObjectPtr<UQuestNode>> ChildNodes;

#if WITH_EDITORONLY_DATA
	/** Color tint used for the node header on the visual graph canvas */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Node")
	FLinearColor NodeColor = FLinearColor(0.2f, 0.65f, 0.35f, 1.0f);
#endif

	/** Indicates if this node is an initial root step for a quest chain */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Node")
	bool bIsRootNode = false;

	/** Object tag identifier required to complete this objective */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FName TargetTag;
};