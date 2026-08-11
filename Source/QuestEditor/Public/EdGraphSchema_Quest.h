#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_Quest.generated.h"

/**
 * FEdGraphSchemaAction_NewQuestNode
 * Action class executed when selecting "Add Quest Node" from the graph context menu.
 */
USTRUCT()
struct FEdGraphSchemaAction_NewQuestNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FEdGraphSchemaAction_NewQuestNode() : FEdGraphSchemaAction() {}
	FEdGraphSchemaAction_NewQuestNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
};


/**
 * UEdGraphSchema_Quest
 * Graph schema defining rules, pin colors, and context menu actions for UEdGraph_Quest.
 */
UCLASS()
class QUESTEDITOR_API UEdGraphSchema_Quest : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	/** Populate right-click context menu actions (e.g. "Add Quest Node") */
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	/** Determine whether a valid connection can be drawn between Pin A and Pin B */
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	/** Returns the color used to draw pins and connection wires */
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
};
