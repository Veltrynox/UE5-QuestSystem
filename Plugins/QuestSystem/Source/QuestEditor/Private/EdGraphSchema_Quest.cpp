#include "EdGraphSchema_Quest.h"
#include "EdGraphNode_Quest.h"
#include "QuestNode.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphNode_QuestStart.h"
#include "EdGraphNode_QuestEnd.h"
#include "EdGraphNode_Comment.h"

UEdGraphNode* FEdGraphSchemaAction_NewQuestNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	ParentGraph->Modify();

	UEdGraphNode_Quest* NewNode = NewObject<UEdGraphNode_Quest>(ParentGraph);
	NewNode->CreateNewGuid();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	UQuestNode* NewQuestNode = NewObject<UQuestNode>(ParentGraph->GetOuter(), NAME_None, RF_Transactional);
	NewQuestNode->StepID = FName(*FString::Printf(TEXT("Step_%d"), FMath::RandRange(100, 999)));
	NewNode->QuestNode = NewQuestNode;

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

	const FText StartMenuDesc = FText::FromString(TEXT("Add Quest Start Node"));
	const FText StartToolTip = FText::FromString(TEXT("Create a golden entry point start node in this graph"));
	TSharedPtr<FEdGraphSchemaAction_NewQuestStartNode> StartAction = MakeShared<FEdGraphSchemaAction_NewQuestStartNode>(Category, StartMenuDesc, StartToolTip, 0);
	ContextMenuBuilder.AddAction(StartAction);

	const FText EndMenuDesc = FText::FromString(TEXT("Add Quest End Node"));
	const FText EndToolTip = FText::FromString(TEXT("Create a red terminal end node in this graph"));
	TSharedPtr<FEdGraphSchemaAction_NewQuestEndNode> EndAction = MakeShared<FEdGraphSchemaAction_NewQuestEndNode>(Category, EndMenuDesc, EndToolTip, 1);
	ContextMenuBuilder.AddAction(EndAction);
	
	const FText MenuDesc = FText::FromString(TEXT("Add Quest Step Node"));
	const FText ToolTip = FText::FromString(TEXT("Create a new quest step node in this graph"));
	TSharedPtr<FEdGraphSchemaAction_NewQuestNode> Action = MakeShared<FEdGraphSchemaAction_NewQuestNode>(Category, MenuDesc, ToolTip, 2);
	ContextMenuBuilder.AddAction(Action);

	const FText CommentCategory = FText::FromString(TEXT("Organize"));
	const FText CommentMenuDesc = FText::FromString(TEXT("Add Comment Block"));
	const FText CommentToolTip = FText::FromString(TEXT("Create a resizable comment box frame to organize quest nodes"));
	TSharedPtr<FEdGraphSchemaAction_NewNode> CommentAction = MakeShared<FEdGraphSchemaAction_NewNode>(CommentCategory, CommentMenuDesc, CommentToolTip, 0);
	CommentAction->NodeTemplate = NewObject<UEdGraphNode_Comment>();
	ContextMenuBuilder.AddAction(CommentAction);
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

UEdGraphNode* FEdGraphSchemaAction_NewQuestStartNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	ParentGraph->Modify();

	UEdGraphNode_QuestStart* NewNode = NewObject<UEdGraphNode_QuestStart>(ParentGraph);
	NewNode->CreateNewGuid();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	UQuestNode* NewQuestNode = NewObject<UQuestNode>(ParentGraph->GetOuter(), NAME_None, RF_Transactional);
	NewQuestNode->StepID = FName(*FString::Printf(TEXT("Start_%d"), FMath::RandRange(100, 999)));
	NewQuestNode->bIsRootNode = true;
	NewQuestNode->NodeColor = NewNode->GetNodeTitleColor();
	NewNode->QuestNode = NewQuestNode;

	NewNode->AllocateDefaultPins();
	ParentGraph->AddNode(NewNode, true, bSelectNewNode);

	if (FromPin)
	{
		NewNode->GetSchema()->TryCreateConnection(FromPin, NewNode->GetPinAt(0));
	}

	return NewNode;
}

UEdGraphNode* FEdGraphSchemaAction_NewQuestEndNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	ParentGraph->Modify();

	UEdGraphNode_QuestEnd* NewNode = NewObject<UEdGraphNode_QuestEnd>(ParentGraph);
	NewNode->CreateNewGuid();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	UQuestNode* NewQuestNode = NewObject<UQuestNode>(ParentGraph->GetOuter(), NAME_None, RF_Transactional);
	NewQuestNode->StepID = FName(*FString::Printf(TEXT("End_%d"), FMath::RandRange(100, 999)));
	NewQuestNode->bIsRootNode = false;
	NewQuestNode->NodeColor = NewNode->GetNodeTitleColor();
	NewNode->QuestNode = NewQuestNode;

	NewNode->AllocateDefaultPins();
	ParentGraph->AddNode(NewNode, true, bSelectNewNode);

	if (FromPin)
	{
		NewNode->GetSchema()->TryCreateConnection(FromPin, NewNode->GetPinAt(0));
	}

	return NewNode;
}
