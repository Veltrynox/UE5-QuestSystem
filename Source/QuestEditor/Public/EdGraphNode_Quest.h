#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "Quest/QuestNode.h"
#include "EdGraphNode_Quest.generated.h"

UCLASS()
class QUESTEDITOR_API UEdGraphNode_Quest : public UEdGraphNode
{
	GENERATED_BODY()

public:
	/** Reference to the underlying runtime UQuestNode data asset object */
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Quest Node")
	TObjectPtr<UQuestNode> QuestNode;

	virtual void NodeConnectionListChanged() override;
	
	/** Create input and output pins for connecting nodes */
	virtual void AllocateDefaultPins() override;

	/** Returns the title text displayed at the top of the visual node box */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	/** Returns the background header color for the visual node title bar */
	virtual FLinearColor GetNodeTitleColor() const override;

	/** Allows users to delete this node from the graph canvas */
	virtual bool CanUserDeleteNode() const override { return true; }
};
