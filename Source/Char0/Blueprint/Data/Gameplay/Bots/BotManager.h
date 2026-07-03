#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BotManager.generated.h"

class ASpawnArea;
struct FSpawn;
struct FWaveInfo;
class ASpawnPoint;
class ABotCharacter;

/**
 *
 * wave Bots	SpawnPoint
 * 0	10		0-9
 * 1	20		10-29
 * 2	30		30-59
 * 3	40		60-99
 * 4	50		100-149
 * 5	60		150-209
 * 6	70		210-279
 * 7	80		280-359
 * 8	90		360-449
 * 9	100		450-549
 * 10	150		210-359
 *
 */
UCLASS()
class CHAR0_API UBotManager : public UObject
{
	GENERATED_BODY()
	/**
	 * 0-9		Car 1
	 * 10-19	Car 2
	 * 20-29	Car 3
	 * 30-39	Car 4
	 * 40-49	Car 5
	 * 50-59	Car 6
	 * 60-69	Car 7
	 * 70-79	Car 8
	 * 80-99	Car 9
	 * 90-99	Car 10
	 * 0-149
	 */
	UPROPERTY()
	TMap<int32, ABotCharacter*> BotPool;

	/**
	 * 0-9		Car 2 (10)
	 * 10-29	Car 3 (20)
	 * 30-59	Car 4 (30)
	 * 60-99	Car 5 (40)
	 * 100-149	Car 6 (50)
	 * 150-209	Car 7 (60)
	 * 210-279	Car 8 (70)
	 * 280-359	Car 9 (80)
	 * 360-449	Car 10 (90)
	 * 210-359	Final Wave
	 */

	UPROPERTY()
	TSubclassOf<ABotCharacter> BotClass;

	UPROPERTY()
	int SpawnIndex = -1;

	TMap<int32, TPair<ASpawnArea*, ASpawnArea*>> Spawns;

	UPROPERTY()
	TArray<FSpawn> ToSpawn;

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

public:
	virtual UWorld* GetWorld() const override;

	void SetBotClass(TSubclassOf<ABotCharacter> Class);
	void SetSpawnAreas(const TMap<int32, TTuple<ASpawnArea*, ASpawnArea*>>& SpawnAreas);
	void UpdateBotSpawnLocation(int ClosestToHead);
	
	UFUNCTION()
	void ActivateBotsForWave(const int32 Level, FWaveInfo WaveInfo);

private:
	void StartSpawnBots();
	static int32 GetSpawnAmount(int32 WaveIndex);
	ASpawnArea* GetSpawnArea(int Index);

	UFUNCTION()
	void Spawn();
};
