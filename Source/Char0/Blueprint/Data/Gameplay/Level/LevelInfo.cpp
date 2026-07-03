
#include "LevelInfo.h"

#include <string>

#include "Char0/Blueprint/Data/Gameplay/Wave/WaveInfo.h"

FLevelInfo::FLevelInfo()
{
	Level = 0;
	CurrentWave = 0;
}

FLevelInfo::FLevelInfo(const int32 Lvl)
{
	Level = Lvl; 
	CurrentWave = 0;
	
	for (int i = 0; i <= 10; i++)
	{
		Waves.Add(FWaveInfo(i));
	}
}

FWaveInfo FLevelInfo::DecrementBot()
{
	Waves[CurrentWave].BotDestroyed();
	return Waves[CurrentWave];
}

bool FLevelInfo::IsComplete()
{
	for (const FWaveInfo Wave : Waves)
	{
		if (!Wave.AllDestroyed())
		{
			return false;
		}
	}
	return true;
}

bool FLevelInfo::HasNextWave() const
{
	return CurrentWave < Waves.Num();
}

void FLevelInfo::SetNextWave()
{
	CurrentWave++;
}

FWaveInfo FLevelInfo::StartWave()
{
	return Waves[CurrentWave];
}

void FLevelInfo::Next()
{
	Level++; 
	CurrentWave = 0;
	
	for (int i = 0; i <= 10; i++)
	{
		Waves.Add(FWaveInfo(i));
	}
}

FString FLevelInfo::ToString()
{
	FString ReturnString = FString("Level #") + FString(std::to_string(Level).c_str());
	for (const auto& Wave : Waves)
	{
		ReturnString += "\n\t" + Wave.ToString();;
	}

	return ReturnString;
}
