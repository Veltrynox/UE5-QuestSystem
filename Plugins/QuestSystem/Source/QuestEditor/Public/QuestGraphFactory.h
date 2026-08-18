#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestGraphFactory.generated.h"

/**
 * UQuestGraphFactory
 * Custom factory class responsible for instantiating new UQuestGraph asset objects.
 */
UCLASS()
class QUESTEDITOR_API UQuestGraphFactory : public UFactory
{
	GENERATED_BODY()

public:
	UQuestGraphFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
