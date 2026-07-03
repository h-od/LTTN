#pragma once

#include "CoreMinimal.h"
#include "Level/LevelInfo.h"
#include "UObject/Object.h"
#include "GameplayManager.generated.h"

class ULevelInfo;
class UWaveInfo;

USTRUCT()
struct CHAR0_API FGameplayManager
{
	GENERATED_BODY()
	
private:
public:
	UPROPERTY()
	FLevelInfo Level = FLevelInfo(0);
private:
	
	UPROPERTY()
	TArray<FLevelInfo> CompletedLevels;
	
public:
	FWaveInfo DecrementBot();
	void SetNextWave();
	bool IsLevelComplete();
	void StartLevel();
	bool HasNextWave() const;
	FWaveInfo StartWave();
	
	TArray<FLevelInfo> GetAllLevels(); // called at the end to show player
	
	FString ToString();
};
