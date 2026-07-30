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
	
	UPROPERTY()
	FLevelInfo Level = FLevelInfo();
	
	FWaveInfo DecrementBot();
	void SetNextWave();
	FWaveInfo StartWave();
	
	FString ToString();
};
