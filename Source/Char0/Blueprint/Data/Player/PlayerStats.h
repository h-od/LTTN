#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerStats.generated.h"

USTRUCT()
struct CHAR0_API FPlayerStats
{
	GENERATED_BODY()
	FPlayerStats() : Id(-1)
	{
	}

	explicit FPlayerStats(const int32 I)
	{
		Id = I;
	}
	
	UPROPERTY()
	int32 Id;
	UPROPERTY()
	FText DisplayName = FText::GetEmpty();
	UPROPERTY()
	int32 TimesDowned = 0;
	UPROPERTY()
	int32 Revives = 0;
	UPROPERTY()
	int32 Destroyed = 0;
	UPROPERTY()
	int32 PointsEarned = 0;
	UPROPERTY()
	int32 PointsSpent = 0;
};
