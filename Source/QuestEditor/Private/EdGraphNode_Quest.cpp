#include "EdGraphNode_Quest.h"

void UEdGraphNode_Quest::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	
	if (!QuestNode) return;

	// Update root node status based on input pin connections
	UEdGraphPin* InputPin = GetPinAt(0);
	if (InputPin)
	{
		QuestNode->bIsRootNode = (InputPin->LinkedTo.Num() == 0);
	}
	
	// Update child node links from output pin connections
	QuestNode->ChildNodes.Empty();
	UEdGraphPin* OutputPin = GetPinAt(1);
	if (!OutputPin) return;
	
	for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
	{
		if (!LinkedPin) continue;
		UEdGraphNode* OwningNode = LinkedPin->GetOwningNode();
		UEdGraphNode_Quest* TargetEdNode = Cast<UEdGraphNode_Quest>(OwningNode);
		if (TargetEdNode && TargetEdNode->QuestNode)
		{
			QuestNode->ChildNodes.Add(TargetEdNode->QuestNode);
		}
	}
}

void UEdGraphNode_Quest::AllocateDefaultPins()
{
	// Create an Input pin on the left side of the node
	CreatePin(EGPD_Input, FName("QuestPin"), FName("In"));

	// Create an Output pin on the right side of the node
	CreatePin(EGPD_Output, FName("QuestPin"), FName("Out"));
}

FText UEdGraphNode_Quest::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (QuestNode && !QuestNode->StepID.IsNone())
	{
		return FText::FromName(QuestNode->StepID);
	}

	return FText::FromString(TEXT("Quest Node"));
}

FLinearColor UEdGraphNode_Quest::GetNodeTitleColor() const
{
	if (QuestNode)
	{
		return QuestNode->NodeColor;
	}

	return FLinearColor(0.2f, 0.65f, 0.35f, 1.0f);
}