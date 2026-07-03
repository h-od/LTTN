#include "WaveInfo.h"

#include <string>

void FWaveInfo::BotDestroyed()
{
	Destroyed++;
}

bool FWaveInfo::AllDestroyed() const
{
	return Total == Destroyed;
}

FString FWaveInfo::ToString() const
{
	return FString("Wave #") + FString(std::to_string(Index).c_str())
		+ "\n\t Total: " + FString(std::to_string(Total).c_str())
		+ "\n\t Destroyed: " + FString(std::to_string(Destroyed).c_str());
}
