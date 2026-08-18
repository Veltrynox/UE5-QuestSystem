#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "QuestNode.h"
#include "EdGraphNode_Quest.generated.h"

UCLASS()
class QUESTEDITOR_API UEdGraphNode_Quest : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Quest Node")
	TObjectPtr<UQuestNode> QuestNode;

	virtual void NodeConnectionListChanged() override;
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }
};
