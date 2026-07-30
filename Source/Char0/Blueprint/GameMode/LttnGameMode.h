#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Data/Gameplay/GameplayManager.h"
#include "GameFramework/GameMode.h"
#include "LttnGameMode.generated.h"

class AStartPawn;
class ALttnCharacter;
class ASpawnArea;
class ALttnController;
class ALttnGameState;
class ABotCharacter;
class ADoor;
class UBotManager;

UCLASS()
class CHAR0_API ALttnGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY()
	ALttnGameState* State;

	UPROPERTY()
	FGameplayManager GameplayManager;

	UPROPERTY()
	UBotManager* BotManager;

	UPROPERTY()
	FTimerHandle NextWaveTimerHandle;

	UPROPERTY()
	int32 CurrentLevel = 0;

	UPROPERTY()
	TMap<int32, int32> PlayersLocation;

	UPROPERTY()
	TMap<int32, bool> PlayersAlive;

	UPROPERTY()
	TArray<ALttnController*> Players;

	UPROPERTY()
	TMap<int32, ADoor*> Doors;
	
	UPROPERTY()
	TArray<ASpawnArea*> PlayerSpawn;//TODO TArray<FVector>
	
	UPROPERTY()
	TArray<ASpawnArea*> PlayerReSpawn; //TODO TArray<FVector>

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABotCharacter> BotClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALttnCharacter> PlayerCharacterClass;

public:
	void Spawn(ALttnController* LttnController, bool bRespawn);
	
	void DecrementBots(int32 PlayerId);

	void UpdatePlayerLocation(int32 PlayerId, int32 Location);

	void UpdateBotSpawnLocation();

	void StartGame();

	void CheckForDeadPlayers();

	void PlayerDead(int32 PlayerId);
	
	APawn* GetPlayerPawn(int32 PlayerId);

	void RevivePlayer(int32 RevivingPlayerId, int32 PlayerToReviveId);
	
	void OpenDoor(int32 DoorNumber);

protected:
	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	void FindAndSetDoors();

private:
	void FindAndSetSpawnAreas();

	void StartWave();

	UFUNCTION(Server, Reliable)
	void Server_UpdateBotSpawnLocation();
	void Server_UpdateBotSpawnLocation_Implementation();

	bool AllDead();
};
