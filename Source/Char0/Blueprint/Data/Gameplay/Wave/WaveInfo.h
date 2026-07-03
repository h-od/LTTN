#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "WaveInfo.generated.h"

USTRUCT()
struct CHAR0_API FWaveInfo
{
	GENERATED_BODY()
	
	FWaveInfo() : Index(-1), Total(-1), Destroyed(-1)
	{
	}

	explicit FWaveInfo(const int32 I)
	{
		Index = I;
		Destroyed = 0;

		if (Index < 10)
		{
			Total = (I + 1) * 10;
		}
		else
		{
			Total = 250;
		}
	}

	UPROPERTY()
	int32 Index;
	UPROPERTY()
	int32 Total;
	UPROPERTY()
	int32 Destroyed;

	void BotDestroyed();
	bool AllDestroyed() const;
	FString ToString() const;
};
