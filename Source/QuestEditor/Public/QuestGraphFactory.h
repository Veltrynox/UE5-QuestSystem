#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestGraphFactory.generated.h"

/**
 * UQuestGraphFactory
 * Custom factory class responsible for instantiating new UQuestGraph asset objects.
 * Enables the "Add/New Asset" context menu option in the Content Browser.
 */
UCLASS()
class QUESTEDITOR_API UQuestGraphFactory : public UFactory
{
	GENERATED_BODY()

public:
	/** Default constructor setting factory configuration flags */
	UQuestGraphFactory();

	/**
	 * Factory creation method called by Unreal Editor when creating a new asset instance.
	 *
	 * @param InClass   Target class to instantiate (UQuestGraph)
	 * @param InParent  Outer package object destination for the new asset
	 * @param InName    Asset file name specified by the user
	 * @param Flags     Object flags to apply to the newly created asset
	 * @param Context   Optional context object
	 * @param Warn      Feedback context for logging warnings/errors
	 * @return Newly created UQuestGraph instance
	 */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};