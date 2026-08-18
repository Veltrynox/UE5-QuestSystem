#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_Quest.generated.h"

USTRUCT()
struct FEdGraphSchemaAction_NewQuestNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FEdGraphSchemaAction_NewQuestNode() : FEdGraphSchemaAction() {}
	FEdGraphSchemaAction_NewQuestNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
};

USTRUCT()
struct FEdGraphSchemaAction_NewQuestStartNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FEdGraphSchemaAction_NewQuestStartNode() : FEdGraphSchemaAction() {}
	FEdGraphSchemaAction_NewQuestStartNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
};

USTRUCT()
struct FEdGraphSchemaAction_NewQuestEndNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FEdGraphSchemaAction_NewQuestEndNode() : FEdGraphSchemaAction() {}
	FEdGraphSchemaAction_NewQuestEndNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
};

UCLASS()
class QUESTEDITOR_API UEdGraphSchema_Quest : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
};
