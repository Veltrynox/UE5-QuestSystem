#include "EdGraphNode_QuestStart.h"

void UEdGraphNode_QuestStart::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, FName("QuestPin"), FName("Out"));
}

FLinearColor UEdGraphNode_QuestStart::GetNodeTitleColor() const
{
	return FLinearColor(0.95f, 0.65f, 0.1f, 1.0f);
}

FText UEdGraphNode_QuestStart::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (QuestNode && !QuestNode->StepID.IsNone())
	{
		return FText::FromString(FString::Printf(TEXT("[START] %s"), *QuestNode->StepID.ToString()));
	}

	return FText::FromString(TEXT("[START] Quest Entry"));
}
