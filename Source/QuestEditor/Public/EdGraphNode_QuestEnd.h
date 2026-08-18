#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_Quest.h"
#include "EdGraphNode_QuestEnd.generated.h"

/**
 * UEdGraphNode_QuestEnd
 * Dedicated terminal End Node for quest graphs with no Output pin.
 */
UCLASS()
class QUESTEDITOR_API UEdGraphNode_QuestEnd : public UEdGraphNode_Quest
{
	GENERATED_BODY()

public:
	/** Creates only an Input pin (no Output pin) */
	virtual void AllocateDefaultPins() override;

	/** Returns a red header color for end nodes */
	virtual FLinearColor GetNodeTitleColor() const override;

	/** Returns [END] prefixed node title */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
