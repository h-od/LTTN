#include "LttnGameMode.h"

#include "Char0/Blueprint/Actor/Door/Door.h"
#include "Char0/Blueprint/Actor/Spawn/SpawnArea.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Controller/LttnController.h"
#include "Char0/Blueprint/Data/Gameplay/Bots/BotManager.h"
#include "Char0/Blueprint/State/Game/LttnGameState.h"
#include "Kismet/GameplayStatics.h"

void ALttnGameMode::Spawn(ALttnController* LttnController, bool bRespawn)
{
	FTransform SpawnTransform;
	if (bRespawn)
	{
		SpawnTransform = PlayerReSpawn[LttnController->Id]->GetTransform();
	}
	else
	{
		SpawnTransform = PlayerSpawn[LttnController->Id]->GetTransform();
	}
	ALttnCharacter* LttnCharacter = GetWorld()->SpawnActor<ALttnCharacter>(PlayerCharacterClass, SpawnTransform, FActorSpawnParameters());

	if (LttnController->HasRagDoll())
	{
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
	if (GameplayManager.IsLevelComplete())
	{
		State->LevelCompleted();
		State->bCanStartNewLevel = true;
		// also open/close door/ doors
	}
	if (!GameplayManager.IsLevelComplete() and WaveInfo.AllDestroyed() and GameplayManager.HasNextWave())
	{
		GameplayManager.SetNextWave();
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
	for (const TTuple PlayerLocation : PlayersLocation)
	{
		if (PlayerLocation.Value != -1)
		{
			for (ALttnController* Player : Players)
			{
				Player->CantStartGame();
			}
			return;
		}
	}
	// GameplayManager.StartLevel();
	ADoor* ExtDoor = Doors[-1];
	ExtDoor->OnDoorCloseCompleteDelegate.AddDynamic(this, &ALttnGameMode::ExtDoorClosed);
	ExtDoor->CloseDoor();
}

void ALttnGameMode::StartLevel()
{
	UKismetSystemLibrary::PrintString(GetWorld(), "ALttnGameMode::StartLevel: " + FString::FromInt(GameplayManager.Level.Level), true, true, FLinearColor::Red, 25.0f);
	if (GameplayManager.IsLevelComplete())
	{
		State->bCanStartNewLevel = false;
		GameplayManager.StartLevel();
	}

	StartWave();

	for (const auto Player : Players)
	{
		//TODO do we need to rez?
		PlayersAlive[Player->Id] = true;
	}
}

void ALttnGameMode::PlayerDead(const int32 PlayerId)
{
	UpdateBotSpawnLocation();
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
	}
}

void ALttnGameMode::ResPlayer(int32 RessingPlayerId, int32 PlayerToRessId)
{
	for (const auto Player : Players)
	{
		if (Player->Id == PlayerToRessId)
		{
			PlayersAlive[PlayerToRessId] = true;

			Spawn(Player, true);

			State->RevivedPlayer(RessingPlayerId);
		}
	}
}

void ALttnGameMode::BeginPlay()
{
	Super::BeginPlay();
	BotManager = NewObject<UBotManager>(this, UBotManager::StaticClass());
	BotManager->SetBotClass(BotClass);

	GameplayManager = FGameplayManager();

	FindAndSetDoors();
	FindAndSetSpawnAreas();
}

void ALttnGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	State = Cast<ALttnGameState>(GameState);

	ALttnController* LttnController = Cast<ALttnController>(NewPlayer);
	const int32 PlayerId = Players.Num();
	Players.Add(LttnController);
	LttnController->Id = PlayerId;
	UpdatePlayerLocation(PlayerId, -2); // (-2 = lobby)

	PlayersAlive.Add(PlayerId, true); //TODO don't know why this needs to be map?


	FindAndSetSpawnAreas(); //TODO just do once 
	Spawn(LttnController, false);
	// FindAndPossessCamera(LttnController);
}

void ALttnGameMode::FindAndSetDoors()
{
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
	TArray<AActor*> FoundSpawnAreaActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), FoundSpawnAreaActors);

	TMap<int32, TTuple<ASpawnArea*, ASpawnArea*>> SpawnAreas;
	for (AActor* Actor : FoundSpawnAreaActors)
	{
		if (Actor->IsA(ASpawnArea::StaticClass()))
		{
			if (ASpawnArea* SpawnArea = Cast<ASpawnArea>(Actor); SpawnArea->bPlayer)
			{
				if (SpawnArea->bStartSpawn)
				{
					StartSpawnLocation = SpawnArea->GetSpawnPoint();
				}
				else
				{
					if (SpawnArea->bRespawn)
					{
						PlayerReSpawn.Add(SpawnArea);
					}
					else
					{
						PlayerSpawn.Add(SpawnArea);
					}
				}
			}
			else
			{
				if (int32 InKey = SpawnArea->Index; SpawnAreas.Contains(InKey))
				{
					SpawnAreas.Add(InKey, TTuple<ASpawnArea*, ASpawnArea*>(SpawnAreas[InKey].Key, SpawnArea));
				}
				else
				{
					SpawnAreas.Add(InKey, TTuple<ASpawnArea*, ASpawnArea*>(SpawnArea, nullptr));
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
	UKismetSystemLibrary::PrintString(GetWorld(), "ALttnGameMode::StartWave Starting Wave: " + FString::FromInt(Wave.Index), true, true, FLinearColor::Red, 5.0f);
	OpenDoor(Wave.Index + 1);
	BotManager->ActivateBotsForWave(CurrentLevel, Wave);
}

void ALttnGameMode::Server_UpdateBotSpawnLocation_Implementation()
{
	int ClosestToHead = 11;
	for (const TTuple Location : PlayersLocation)
	{
		if (Location.Value < ClosestToHead)
		{
			ClosestToHead = Location.Value;
		}
	}

	if (BotManager and ClosestToHead != 11)
	{
		BotManager->UpdateBotSpawnLocation(ClosestToHead);
	}
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

void ALttnGameMode::OpenDoor(const int32 DoorToOpen)
{
	Doors[DoorToOpen]->OpenDoor();
}

void ALttnGameMode::ExtDoorClosed()
{
	OpenDoor(0);
	State->bGameStarted = true;
	Doors[-1]->OnDoorCloseCompleteDelegate.RemoveDynamic(this, &ALttnGameMode::ExtDoorClosed);
}
