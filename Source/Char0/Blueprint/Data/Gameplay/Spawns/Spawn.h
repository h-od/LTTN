#pragma once

#include "Spawn.generated.h"

USTRUCT()
struct CHAR0_API FSpawn
{
	GENERATED_BODY()

	FSpawn() : Index(0), Wave(0)
	{
	}

	explicit FSpawn(const int32 I, const int32 WaveI) : Index(I), Wave(WaveI)
	{
	}

	bool operator==(const FSpawn& Other) const
	{
		return Index == Other.Index;
	}

	UPROPERTY()
	int32 Index;
	UPROPERTY()
	int32 Wave;
};
