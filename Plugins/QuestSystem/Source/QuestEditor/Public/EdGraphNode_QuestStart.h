#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_Quest.h"
#include "EdGraphNode_QuestStart.generated.h"

UCLASS()
class QUESTEDITOR_API UEdGraphNode_QuestStart : public UEdGraphNode_Quest
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
