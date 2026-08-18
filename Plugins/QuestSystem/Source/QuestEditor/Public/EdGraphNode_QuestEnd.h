#pragma once

#include "CoreMinimal.h"
#include "EdGraphNode_Quest.h"
#include "EdGraphNode_QuestEnd.generated.h"

UCLASS()
class QUESTEDITOR_API UEdGraphNode_QuestEnd : public UEdGraphNode_Quest
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
