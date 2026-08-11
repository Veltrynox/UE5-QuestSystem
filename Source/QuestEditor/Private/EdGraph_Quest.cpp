#include "EdGraph_Quest.h"
#include "EdGraphNode_Quest.h"
#include "Quest/QuestGraph.h"

void UEdGraph_Quest::RebuildQuestGraph()
{
	UQuestGraph* QuestGraph = Cast<UQuestGraph>(GetOuter());
	if (!QuestGraph) return;

	// Mark parent package dirty for saving
	QuestGraph->Modify();
	QuestGraph->MarkPackageDirty();
	
	QuestGraph->AllNodes.Empty();
	QuestGraph->RootNodes.Empty();

	// Synchronize runtime nodes and child connections from visual graph canvas
	for (UEdGraphNode* Node : Nodes)
	{
		if (UEdGraphNode_Quest* QuestEdNode = Cast<UEdGraphNode_Quest>(Node))
		{
			if (UQuestNode* QuestNode = QuestEdNode->QuestNode)
			{
				QuestGraph->AllNodes.Add(QuestNode);

				if (QuestNode->bIsRootNode)
				{
					QuestGraph->RootNodes.Add(QuestNode);
				}
				
				// Rebuild child node connections from visual pin links
				QuestNode->ChildNodes.Empty();
				UEdGraphPin* OutputPin = QuestEdNode->GetPinAt(1);
				if (OutputPin)
				{
					for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
					{
						if (UEdGraphNode_Quest* TargetEdNode = Cast<UEdGraphNode_Quest>(LinkedPin->GetOwningNode()))
						{
							if (TargetEdNode->QuestNode)
							{
								QuestNode->ChildNodes.Add(TargetEdNode->QuestNode);
							}
						}
					}
				}
			}
		}
	}
}

void UEdGraph_Quest::NotifyGraphChanged()
{
	Super::NotifyGraphChanged();
	RebuildQuestGraph();
}