#pragma once

#include "Spawn.generated.h"

USTRUCT()
struct CHAR0_API FSpawn
{
	GENERATED_BODY()
	
	FSpawn() : Level(0), Index(0), WaveIndex(0)
	{
	}

	explicit FSpawn(const int32 Lev, const int32 I, const int32 WaveI) : Level(Lev), Index(I), WaveIndex(WaveI)
	{
	}

	bool operator==(const FSpawn& Other) const
	{
		return Level == Other.Level && Index == Other.Index;
	}
	
	UPROPERTY()
	int32 Level;
	UPROPERTY()
	int32 Index;
	UPROPERTY()
	int32 WaveIndex;
};
