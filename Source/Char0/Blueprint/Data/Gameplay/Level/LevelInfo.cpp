#include "LevelInfo.h"

#include <string>

#include "Char0/Blueprint/Data/Gameplay/Wave/WaveInfo.h"

FLevelInfo::FLevelInfo()
{
	CurrentWave = 0;
	Waves.Add(FWaveInfo(CurrentWave));
}

FWaveInfo FLevelInfo::DecrementBot()
{
	Waves[CurrentWave].BotDestroyed();
	return Waves[CurrentWave];
}

void FLevelInfo::SetNextWave()
{
	// CurrentWave++;
	Waves.Add(FWaveInfo(++CurrentWave));
}

FWaveInfo FLevelInfo::StartWave()
{
	return Waves[CurrentWave];
}

FString FLevelInfo::ToString()
{
	FString ReturnString = FString("Current Wave #") + FString::FromInt(CurrentWave);
	for (const auto& Wave : Waves)
	{
		ReturnString += "\n\t" + Wave.ToString();;
	}

	return ReturnString;
}
