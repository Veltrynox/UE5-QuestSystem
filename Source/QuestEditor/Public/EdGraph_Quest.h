#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_Quest.generated.h"

/**
 * UEdGraph_Quest
 * Custom UEdGraph implementation for the Quest Editor.
 * Serves as the editor canvas container holding visual node objects (UEdGraphNode_Quest).
 */
UCLASS()
class QUESTEDITOR_API UEdGraph_Quest : public UEdGraph
{
	GENERATED_BODY()
	
	public:
		void RebuildQuestGraph();
		virtual void NotifyGraphChanged() override;
};
