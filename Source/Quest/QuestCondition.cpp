#include "QuestCondition.h"
#include "QuestManager.h"
#include "QuestGraph.h"

bool UQuestCondition::IsConditionMet_Implementation(UQuestManager* Manager, UQuestNode* OwningNode) const
{
	return true;
}

bool UQuestCondition_CheckNodeState::IsConditionMet_Implementation(UQuestManager* Manager, UQuestNode* OwningNode) const
{
	if (!Manager || !Manager->ActiveQuestGraph || TargetStepID.IsNone())
	{
		return true;
	}

	for (UQuestNode* Node : Manager->ActiveQuestGraph->AllNodes)
	{
		if (Node && Node->StepID == TargetStepID)
		{
			bool bMatches = (Node->Status == RequiredStatus);
			return bInvertCondition ? !bMatches : bMatches;
		}
	}

	return bInvertCondition;
}

bool UQuestCondition_CheckWorldTag::IsConditionMet_Implementation(UQuestManager* Manager, UQuestNode* OwningNode) const
{
	if (!Manager || WorldTag.IsNone())
	{
		return true;
	}

	bool bIsPresent = Manager->ActiveWorldTags.Contains(WorldTag);
	return bInvertCondition ? !bIsPresent : bIsPresent;
}
