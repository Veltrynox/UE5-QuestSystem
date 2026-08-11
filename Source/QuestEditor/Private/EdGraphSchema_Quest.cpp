#include "EdGraphSchema_Quest.h"
#include "EdGraphNode_Quest.h"
#include "Quest/QuestNode.h"
#include "EdGraph/EdGraph.h"

UEdGraphNode* FEdGraphSchemaAction_NewQuestNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	ParentGraph->Modify();

	// Instantiate new visual node object
	UEdGraphNode_Quest* NewNode = NewObject<UEdGraphNode_Quest>(ParentGraph);
	NewNode->CreateNewGuid();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	// Instantiate underlying runtime data object
	UQuestNode* NewQuestNode = NewObject<UQuestNode>(ParentGraph->GetOuter(), NAME_None, RF_Transactional);
	NewQuestNode->StepID = FName(*FString::Printf(TEXT("Step_%d"), FMath::RandRange(100, 999)));
	NewNode->QuestNode = NewQuestNode;

	// Allocate pins and register node with graph
	NewNode->AllocateDefaultPins();
	ParentGraph->AddNode(NewNode, true, bSelectNewNode);

	if (FromPin)
	{
		if (UEdGraphNode_Quest* ParentEdNode = Cast<UEdGraphNode_Quest>(FromPin->GetOwningNode()))
		{
			if (ParentEdNode->QuestNode)
			{
				NewQuestNode->NodeColor = ParentEdNode->QuestNode->NodeColor;
			}
		}

		NewNode->GetSchema()->TryCreateConnection(FromPin, NewNode->GetPinAt(0));
	}

	return NewNode;
}


void UEdGraphSchema_Quest::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);

	const FText Category = FText::FromString(TEXT("Quest Graph"));
	const FText MenuDesc = FText::FromString(TEXT("Add Quest Step Node"));
	const FText ToolTip = FText::FromString(TEXT("Create a new quest step node in this graph"));

	TSharedPtr<FEdGraphSchemaAction_NewQuestNode> Action = MakeShared<FEdGraphSchemaAction_NewQuestNode>(Category, MenuDesc, ToolTip, 0);
	ContextMenuBuilder.AddAction(Action);
}


const FPinConnectionResponse UEdGraphSchema_Quest::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pins"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pin to same node"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Must connect output to input"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Connect pins"));
}

FLinearColor UEdGraphSchema_Quest::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor(0.2f, 0.8f, 0.4f, 1.0f);
}
