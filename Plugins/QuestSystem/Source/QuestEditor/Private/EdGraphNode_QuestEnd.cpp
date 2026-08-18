#include "EdGraphNode_QuestEnd.h"

void UEdGraphNode_QuestEnd::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, FName("QuestPin"), FName("In"));
}

FLinearColor UEdGraphNode_QuestEnd::GetNodeTitleColor() const
{
	return FLinearColor(0.85f, 0.2f, 0.2f, 1.0f);
}

FText UEdGraphNode_QuestEnd::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (QuestNode && !QuestNode->StepID.IsNone())
	{
		return FText::FromString(FString::Printf(TEXT("[END] %s"), *QuestNode->StepID.ToString()));
	}

	return FText::FromString(TEXT("[END] Quest Finish"));
}
