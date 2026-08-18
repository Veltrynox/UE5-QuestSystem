#include "EdGraphNode_Quest.h"

void UEdGraphNode_Quest::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	
	if (!QuestNode) return;

	// Find the input pin
	UEdGraphPin* InputPin = FindPin(FName("In"), EGPD_Input);
	if (InputPin)
	{
		QuestNode->bIsRootNode = (InputPin->LinkedTo.Num() == 0);
	}
	
	// Find the output pin
	QuestNode->ChildNodes.Empty();
	UEdGraphPin* OutputPin = FindPin(FName("Out"), EGPD_Output);
	if (OutputPin)
	{
		for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
		{
			if (!LinkedPin) continue;
			UEdGraphNode* OwningNode = LinkedPin->GetOwningNode();
			UEdGraphNode_Quest* TargetEdNode = Cast<UEdGraphNode_Quest>(OwningNode);
			if (TargetEdNode && TargetEdNode->QuestNode)
			{
				QuestNode->ChildNodes.AddUnique(TargetEdNode->QuestNode);
			}
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
#if WITH_EDITORONLY_DATA
		if (QuestNode->bIsMainQuest)
		{
			return FText::FromString(FString::Printf(TEXT("⭐ [MAIN QUEST] %s"), *QuestNode->StepID.ToString()));
		}
#endif
		return FText::FromName(QuestNode->StepID);
	}

	return FText::FromString(TEXT("Quest Node"));
}

FLinearColor UEdGraphNode_Quest::GetNodeTitleColor() const
{
	if (QuestNode)
	{
#if WITH_EDITORONLY_DATA
		if (QuestNode->bIsMainQuest)
		{
			// Royal purple tint for main story quest milestones
			return FLinearColor(0.55f, 0.15f, 0.75f, 1.0f);
		}
#endif
		return QuestNode->NodeColor;
	}

	return FLinearColor(0.2f, 0.65f, 0.35f, 1.0f);
}