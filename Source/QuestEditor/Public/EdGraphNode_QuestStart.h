#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_Quest.h"
#include "EdGraphNode_QuestStart.generated.h"

/**
 * UEdGraphNode_QuestStart
 * Dedicated golden Start Node for quest graphs with no Input pin.
 */
UCLASS()
class QUESTEDITOR_API UEdGraphNode_QuestStart : public UEdGraphNode_Quest
{
	GENERATED_BODY()

public:
	/** Creates only an Output pin (no Input pin) */
	virtual void AllocateDefaultPins() override;

	/** Returns a golden amber header color */
	virtual FLinearColor GetNodeTitleColor() const override;

	/** Returns [START] prefixed node title */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
