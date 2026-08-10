#include "QuestGraphFactory.h"
#include "Quest/QuestGraph.h"

UQuestGraphFactory::UQuestGraphFactory()
{
	// Specify the exact UObject subclass supported by this factory
	SupportedClass = UQuestGraph::StaticClass();

	// Enable creating new assets from scratch via Content Browser context menu
	bCreateNew = true;

	// Open the associated asset editor immediately after asset creation
	bEditAfterNew = true;
}

UObject* UQuestGraphFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	// Allocate and initialize a new UQuestGraph object inside the target package
	return NewObject<UQuestGraph>(InParent, InClass, InName, Flags);
}