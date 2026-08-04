#include "BotManager.h"

#include "Char0/Blueprint/Actor/Spawn/SpawnArea.h"
#include "Char0/Blueprint/Character/Bot/BotCharacter.h"
#include "Char0/Blueprint/Data/Gameplay/Spawns/BotSpawns.h"
#include "Char0/Blueprint/Data/Gameplay/Spawns/Spawn.h"
#include "Char0/Blueprint/Data/Gameplay/Wave/WaveInfo.h"

UWorld* UBotManager::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

void UBotManager::SetBotClass(const TSubclassOf<ABotCharacter> Class)
{
	BotClass = Class;
}

void UBotManager::SetSpawnAreas(const TMap<int32, FBotSpawns>& SpawnAreas)
{
	Spawns = SpawnAreas;
}

void UBotManager::UpdateBotSpawnLocations(const TArray<int32>& SpawnLocations)
{
	SpawnIndices = SpawnLocations;
}

void UBotManager::ActivateBotsForWave(const FWaveInfo WaveInfo)
{
	for (int i = 0; i < GetSpawnAmount(WaveInfo.Index); i++)
	{
		ToSpawn.Add(FSpawn(i, WaveInfo.Index));
	}

	StartSpawnBots();
}

void UBotManager::StartSpawnBots()
{
	if (const UWorld* World = GetWorld())
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UBotManager::Spawn);
		World->GetTimerManager().SetTimer(SpawnTimerHandle, Delegate, .5f, true);
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("UBotManager::StartSpawnBots Couldn't GetWorld :("));
	}
}

int32 UBotManager::GetSpawnAmount(const int32 WaveIndex)
{
	switch (WaveIndex)
	{
	case 0:
		return 10;
	case 1:
		return 20;
	case 2:
		return 30;
	case 3:
		return 40;
	case 4:
		return 50;
	case 5:
		return 60;
	case 6:
		return 70;
	case 7:
		return 80;
	case 8:
		return 90;
	case 9:
		return 100;
	default:
		return 250;
	}
}

void UBotManager::Spawn()
{
	bool bSuccess;
	const FSpawn Subject = ToSpawn.Top();
	const FVector SpawnLocation = Spawns[GetNextSpawnIndex()].GetNext()->GetSpawnPoint();

	if (BotPool.Contains(Subject.Index))
	{
		if (ABotCharacter* BotCharacter = BotPool[Subject.Index]; BotCharacter->SetActorLocation(SpawnLocation, false))
		{
			BotCharacter->Activate(Subject.Wave);
			bSuccess = true;
		}
		else
		{
			bSuccess = false;
		}
	}
	else
	{
		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		if (ABotCharacter* Bot = GetWorld()->SpawnActor<ABotCharacter>(BotClass, SpawnLocation, FRotator(), SpawnParams))
		{
			BotPool.Add(Subject.Index, Bot);
			Bot->Activate(Subject.Wave);
			bSuccess = true;
		}
		else
		{
			bSuccess = false;
		}
	}

	if (bSuccess)
	{
		ToSpawn.Remove(Subject);
	}

	if (const UWorld* World = GetWorld(); World and ToSpawn.IsEmpty())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

int32 UBotManager::GetNextSpawnIndex()
{
	if (SpawnIndices.Num() == 1)
	{
		return SpawnIndices[0];
	}

	if (++LastSpawnedLocationIndex == SpawnIndices.Num())
	{
		LastSpawnedLocationIndex = 0;
	}
	
	return SpawnIndices[LastSpawnedLocationIndex];
}
