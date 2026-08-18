#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestNode.h"
#include "QuestGraph.generated.h"

UCLASS(BlueprintType)
class QUEST_API UQuestGraph : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Quest Graph")
	TArray<TObjectPtr<UQuestNode>> AllNodes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Graph")
	TArray<TObjectPtr<UQuestNode>> RootNodes;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UEdGraph> EdGraph;
#endif
};
