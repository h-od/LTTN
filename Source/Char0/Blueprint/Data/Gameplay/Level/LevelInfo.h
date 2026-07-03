
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
public:
	UPROPERTY()
	int32 Level = 0;
private:
	
	UPROPERTY()
	int32 CurrentWave = 0;
	UPROPERTY()
	TArray<FWaveInfo> Waves;

public:
	FLevelInfo();
	explicit FLevelInfo(int32 Lvl);
	
	FWaveInfo DecrementBot();
	bool IsComplete();
	bool HasNextWave() const;
	void SetNextWave();
	FWaveInfo StartWave();
	void Next();

	FString ToString();
};
