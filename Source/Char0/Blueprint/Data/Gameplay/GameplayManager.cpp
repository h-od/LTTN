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

FWaveInfo FGameplayManager::StartWave() 
{
	return Level.StartWave();
}

FString FGameplayManager::ToString() 
{
	return Level.ToString(); 
}
