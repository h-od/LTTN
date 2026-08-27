#include "LttnGameMode.h"

#include "Char0/Blueprint/Actor/Door/Door.h"
#include "Char0/Blueprint/Actor/Spawn/SpawnArea.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Controller/LttnController.h"
#include "Char0/Blueprint/Data/Gameplay/Bots/BotManager.h"
#include "Char0/Blueprint/Data/Gameplay/Spawns/BotSpawns.h"
#include "Char0/Blueprint/State/Game/LttnGameState.h"
#include "Kismet/GameplayStatics.h"

void ALttnGameMode::Spawn(ALttnController* LttnController, const bool bRespawn)
{
	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParameters;
	if (bRespawn)
	{
		SpawnTransform = LttnController->GetPawn()->GetTransform();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	}
	else
	{
		SpawnTransform = PlayerSpawn[LttnController->Id]->GetTransform();
	}
	ALttnCharacter* LttnCharacter = GetWorld()->SpawnActor<ALttnCharacter>(PlayerCharacterClass, SpawnTransform, SpawnParameters);

	LttnCharacter->bIsDead = false;
	if (LttnController->HasRagDoll())
	{
		LttnCharacter->SetPlayerInfo(LttnController->GetPlayerManager());
		LttnController->DestroyRagdoll();
		LttnCharacter->SetMaxHealth();
	}
	else
	{
		LttnCharacter->SetNewPlayerInfo();
	}
	LttnController->Possess(LttnCharacter);
}

void ALttnGameMode::DecrementBots(const int32 PlayerId)
{
	const FWaveInfo WaveInfo = GameplayManager.DecrementBot();
	State->DecrementBot(PlayerId);

	if (WaveInfo.AllDestroyed())
	{
		GameplayManager.SetNextWave();
		State->WaveCompleted();
		GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);
		GetWorldTimerManager().SetTimer(NextWaveTimerHandle, this, &ALttnGameMode::StartWave,
		                                5.0f, false);
	}
}

void ALttnGameMode::UpdatePlayerLocation(const int32 PlayerId, const int32 Location)
{
	PlayersLocation.Add(PlayerId, Location);
	UpdateBotSpawnLocation();
}

void ALttnGameMode::UpdateBotSpawnLocation()
{
	Server_UpdateBotSpawnLocation();
}

void ALttnGameMode::StartGame()
{
	StartWave();

	for (const auto Player : Players)
	{
		//TODO we need to rez
		PlayersAlive[Player->Id] = true;
		UpdatePlayerLocation(Player->Id, 0);
	}
}

void ALttnGameMode::CheckForDeadPlayers()
{
	//todo fix this shite
	bool bAnyDead = false;
	for (const TTuple Alive : PlayersAlive)
	{
		if (!Alive.Value)
		{
			bAnyDead = true;
		}
	}
	if (bAnyDead)
	{
		for (const TTuple Alive : PlayersAlive)
		{
			if (ALttnController* Con = Players[Alive.Key])
			{
				if (Alive.Value)
				{
					Con->DisableSphere();
				}
				else
				{
					Con->EnableSphere();
				}
			}
		}
	}
	else
	{
		for (const TTuple Alive : PlayersAlive)
		{
			Players[Alive.Key]->DisableSphere();
		}
	}
}

void ALttnGameMode::PlayerDead(const int32 PlayerId)
{
	UpdatePlayerLocation(PlayerId, -1);
	PlayersAlive[PlayerId] = false;

	State->PlayerDowned(PlayerId);
	if (AllDead())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		const bool bIsMulti = Players.Num() > 1;
		for (const auto Player : Players)
		{
			Player->GameOver(bIsMulti);
		}
	}
	else
	{
		TArray<int> AlivePlayerIds;
		for (auto AlivePlayerId : PlayersAlive)
		{
			if (AlivePlayerId.Value and AlivePlayerId.Key != PlayerId)
			{
				AlivePlayerIds.Add(AlivePlayerId.Key);
			}
		}
		CheckForDeadPlayers();
		Players[PlayerId]->StartSpectate(AlivePlayerIds.Top());
	}
}

APawn* ALttnGameMode::GetPlayerPawn(const int32 PlayerId)
{
	return Players[PlayerId]->GetPawn();
}

int32 ALttnGameMode::GetNextPawnToSpectate(int32 CurrentId) const
{
	if (++CurrentId == Players.Num())
	{
		CurrentId = 0;
	}

	if (PlayersAlive[CurrentId])
	{
		return CurrentId;
	}
	return GetNextPawnToSpectate(CurrentId);
}

int32 ALttnGameMode::GetPreviousPawnToSpectate(int32 CurrentId) const
{
	if (--CurrentId < 0)
	{
		CurrentId = Players.Num() - 1;
	}

	if (PlayersAlive[CurrentId])
	{
		return CurrentId;
	}
	return GetPreviousPawnToSpectate(CurrentId);
}

void ALttnGameMode::RevivePlayer(const int32 RevivingPlayerId, const int32 PlayerToReviveId)
{
	PlayersAlive[PlayerToReviveId] = true;
	Spawn(Players[PlayerToReviveId], true);
	State->RevivedPlayer(RevivingPlayerId);

	CheckForDeadPlayers();
}

void ALttnGameMode::OpenDoor(const int32 DoorNumber)
{
	Doors[DoorNumber]->OpenDoor();
}

void ALttnGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameplayManager = FGameplayManager();

	FindAndSetDoors();
	FindAndSetSpawnAreas();
}

void ALttnGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	BotManager = NewObject<UBotManager>(this, UBotManager::StaticClass());
	BotManager->SetBotClass(BotClass);
	State = Cast<ALttnGameState>(GameState);

	ALttnController* LttnController = Cast<ALttnController>(NewPlayer);
	const int32 PlayerId = Players.Num();
	Players.Add(LttnController);
	LttnController->Id = PlayerId;
	PlayersAlive.Add(PlayerId, true);
	UpdatePlayerLocation(PlayerId, 0); // Spawn Location


	FindAndSetSpawnAreas(); //todo needed here or is begin play enough?
	Spawn(LttnController, false);
}

void ALttnGameMode::FindAndSetDoors()
{
	if (!Doors.IsEmpty())
	{
		return;
	}
	TArray<AActor*> FoundDoorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundDoorActors);
	for (AActor* Actor : FoundDoorActors)
	{
		if (Actor->IsA(ADoor::StaticClass()))
		{
			ADoor* Door = Cast<ADoor>(Actor);
			int32 InKey = Door->DoorNumber;
			Doors.Add(InKey, Door);
		}
	}
}

void ALttnGameMode::FindAndSetSpawnAreas()
{
	if (BotManager->HasSpawnAreas())
	{
		return;
	}
	TArray<AActor*> FoundSpawnAreaActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), FoundSpawnAreaActors);

	TMap<int32, FBotSpawns> SpawnAreas;
	for (AActor* Actor : FoundSpawnAreaActors)
	{
		if (Actor->IsA(ASpawnArea::StaticClass()))
		{
			if (ASpawnArea* SpawnArea = Cast<ASpawnArea>(Actor); SpawnArea->bPlayer)
			{
				PlayerSpawn.Add(SpawnArea);
			}
			else
			{
				if (SpawnAreas.Contains(SpawnArea->Index))
				{
					SpawnAreas[SpawnArea->Index].Add(SpawnArea);
				}
				else
				{
					SpawnAreas.Add(SpawnArea->Index, FBotSpawns(SpawnArea->Index, SpawnArea));
				}
			}
		}
	}
	if (BotManager)
	{
		BotManager->SetSpawnAreas(SpawnAreas);
	}
}

void ALttnGameMode::StartWave()
{
	const FWaveInfo Wave = GameplayManager.StartWave();
	BotManager->ActivateBotsForWave(Wave);
	for (ALttnController* Player : Players)
	{
		Player->SetWave(Wave.Index);
	}
}

void ALttnGameMode::Server_UpdateBotSpawnLocation_Implementation()
{
	//TODO but not if player dead
	TArray<int32> Locations;
	for (const TTuple Alive : PlayersAlive)
	{
		if (int32 Item = PlayersLocation[Alive.Key]; Alive.Value and Item >= 0)
		{
			Locations.Add(Item);
		}
	}

	BotManager->UpdateBotSpawnLocations(Locations);
}

bool ALttnGameMode::AllDead()
{
	for (const TTuple Alive : PlayersAlive)
	{
		if (Alive.Value)
		{
			return false;
		}
	}
	return true;
}

bool ALttnGameMode::HasActivePlayer() const
{
	for (const TTuple<int, bool> Player : PlayersAlive)
	{
		if (Player.Value)
		{
			return true;
		}
	}
	return !PlayersAlive.IsEmpty();
}

FVector ALttnGameMode::ClosestPlayer(const FVector BotLocation)
{
	TArray<FVector> ActorsToCheck;

	for (const TTuple Player : PlayersAlive)
	{
		if (Player.Value)
		{
			ActorsToCheck.Add(Players[Player.Key]->GetPawn()->GetActorLocation());
		}
	}
	
	
	float Distance = TNumericLimits<float>::Max();
	FVector Location;
	
	for (FVector ToCheck : ActorsToCheck)
	{
		if (const float DistanceFromActor = (BotLocation - ToCheck).SizeSquared(); DistanceFromActor < Distance)
		{
			Distance = DistanceFromActor;
			Location = ToCheck;
		}
	}
	
	return Location;
}
