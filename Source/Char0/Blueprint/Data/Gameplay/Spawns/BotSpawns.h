#pragma once

#include "BotSpawns.generated.h"

class ASpawnArea;

USTRUCT()
struct CHAR0_API FBotSpawns
{
	GENERATED_BODY()

	FBotSpawns() : Room(0), LastIndex(0)
	{
	}

	explicit FBotSpawns(const int32 R, const TObjectPtr<ASpawnArea> F) : Room(R), LastIndex(0)
	{
		Spawn.Add(F);
	}

	explicit FBotSpawns(const int32 R, const int32 I) : Room(R), LastIndex(I)
	{
	}

	bool operator==(const FBotSpawns& Other) const
	{
		return Room == Other.Room;
	}

	UPROPERTY()
	int32 Room;

	UPROPERTY()
	int32 LastIndex = 0;
	
	UPROPERTY()
	TArray<ASpawnArea*> Spawn;

	void Add(ASpawnArea* SpawnArea)
	{
		Spawn.Add(SpawnArea);
	}

	TObjectPtr<ASpawnArea> GetNext()
	{
		TObjectPtr<ASpawnArea> SpawnArea = Spawn[LastIndex];
		if (LastIndex == 3)
		{
			LastIndex = 0;
		}
		else
		{
			LastIndex = LastIndex+1;
		}

		return SpawnArea;
	}
};
