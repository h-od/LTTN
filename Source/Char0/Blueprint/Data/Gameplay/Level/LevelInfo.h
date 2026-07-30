
#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Data/Gameplay/Wave/WaveInfo.h"
#include "UObject/Class.h"
#include "LevelInfo.generated.h"

USTRUCT()
struct CHAR0_API FLevelInfo 
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int32 CurrentWave = 0;
	UPROPERTY()
	TArray<FWaveInfo> Waves;

public:
	FLevelInfo();
	
	FWaveInfo DecrementBot();
	void SetNextWave();
	FWaveInfo StartWave();

	FString ToString();
};
