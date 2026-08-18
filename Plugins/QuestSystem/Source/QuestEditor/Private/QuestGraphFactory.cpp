#include "QuestGraphFactory.h"
#include "QuestGraph.h"

UQuestGraphFactory::UQuestGraphFactory()
{
	SupportedClass = UQuestGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UQuestGraphFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UQuestGraph>(InParent, InClass, InName, Flags);
}
