#include "QuestManager.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("=== QuestManager: Initialized successfully ==="));
}

void UQuestManager::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("=== QuestManager: Deinitialized ==="));
	Super::Deinitialize();
}