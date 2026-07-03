#include "BotManager.h"

#include "Spawn.h"
#include "Char0/Blueprint/Actor/Spawn/SpawnArea.h"
#include "Char0/Blueprint/Character/Bot/BotCharacter.h"
#include "Char0/Blueprint/Data/Gameplay/Wave/WaveInfo.h"

UWorld* UBotManager::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

void UBotManager::SetBotClass(const TSubclassOf<ABotCharacter> Class)
{
	BotClass = Class;
}

void UBotManager::SetSpawnAreas(const TMap<int32, TTuple<ASpawnArea*, ASpawnArea*>>& SpawnAreas)
{
	Spawns = SpawnAreas;
}

void UBotManager::UpdateBotSpawnLocation(const int ClosestToHead)
{
	if (ClosestToHead <= 0)
	{
		SpawnIndex = 0;
	}
	else
	{
		SpawnIndex = ClosestToHead;
	}
}

void UBotManager::ActivateBotsForWave(const int32 Level, const FWaveInfo WaveInfo)
{
	for (int i = 0; i < GetSpawnAmount(WaveInfo.Index); i++)
	{
		ToSpawn.Add(FSpawn(Level, i, WaveInfo.Index));
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
	case 10:
		return 250;
	default:
		//TODO throw exception?
		return 0;
	}
}

ASpawnArea* UBotManager::GetSpawnArea(const int Index)
{
	ASpawnArea* SpawnArea;
	if (Index % 2 == 0)
	{
		SpawnArea = Spawns[SpawnIndex].Key;
	}
	else
	{
		SpawnArea = Spawns[SpawnIndex].Value;
	}
	return SpawnArea;
}

void UBotManager::Spawn()
{
	bool bSuccess;
	const FSpawn Subject = ToSpawn.Top();
	FVector SpawnLocation;

	if (Subject.WaveIndex == 10)
	{
		SpawnLocation = Spawns[12].Key->GetSpawnPoint();
	}
	else
	{
		SpawnLocation = GetSpawnArea(Subject.Index)->GetSpawnPoint();
	}
	
	if (BotPool.Contains(Subject.Index))
	{
		if (ABotCharacter* BotCharacter = BotPool[Subject.Index]; BotCharacter->SetActorLocation(SpawnLocation, false))
		{
			BotCharacter->Activate(Subject.Level);
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
			Bot->Activate(Subject.Level);
			bSuccess = true;
		}
		else
		{
			UE_LOG(LogHAL, Log, TEXT("UBotManager::Spawn failed to spawn: %d"), Subject.Index);
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
