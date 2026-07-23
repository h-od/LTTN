#include "GameplayManager.h"

#include "Level/LevelInfo.h"
#include "Wave/WaveInfo.h"

FWaveInfo FGameplayManager::DecrementBot()
{
	return Level.DecrementBot();
}

void FGameplayManager::SetNextWave()
{
	Level.SetNextWave();
}

bool FGameplayManager::IsLevelComplete()
{
	return Level.IsComplete();
}

void FGameplayManager::StartLevel()
{
	CompletedLevels.Add(Level);
	Level.Next();
}

bool FGameplayManager::HasNextWave() const
{
	return Level.HasNextWave();
}

FWaveInfo FGameplayManager::StartWave() 
{
	return Level.StartWave();
}

TArray<FLevelInfo> FGameplayManager::GetAllLevels()
{
	CompletedLevels.Add(Level);
	return CompletedLevels;
}

FString FGameplayManager::ToString() 
{
	return Level.ToString(); 
}
