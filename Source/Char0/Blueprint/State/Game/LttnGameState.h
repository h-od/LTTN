#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Data/Gameplay/PlayerSummary.h"
#include "GameFramework/GameState.h"
#include "LttnGameState.generated.h"

class UWaveInfo;
class UGameInfo;
class UBotManager;
class ASpawnPoint;
class ABotCharacter;
class UArenaManager;

UCLASS()
class CHAR0_API ALttnGameState : public AGameState
{
	GENERATED_BODY()
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerOne)
	FPlayerSummary PlayerOne;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerTwo)
	FPlayerSummary PlayerTwo;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerThree)
	FPlayerSummary PlayerThree;
	
	UPROPERTY(Replicated)
	int32 LevelsCompleted = 0;

public:
	
	UPROPERTY(Replicated)
	bool bGameStarted = false;

	UPROPERTY(Replicated)
	bool bCanStartNewLevel = true;
	
	UFUNCTION()
	void OnRep_PlayerOne();
	UFUNCTION()
	void OnRep_PlayerTwo();
	UFUNCTION()
	void OnRep_PlayerThree();
	
	void OnPlayerOneUpdated();
	void OnPlayerTwoUpdated();
	void OnPlayerThreeUpdated();
	
	// void PlayerJoined(int32 Id);
	void RevivedPlayer(int32 PlayerId);
	void PlayerDowned(int32 PlayerId);

	void LevelCompleted();
	void DecrementBot(int32 PlayerId);
	void ScoreIncreased(int32 PlayerId, int32 Amount);
	void ScoreDecreased(int32 PlayerId, int32 Amount);

	FString GetSummary(int32 PlayerId, bool bIsMulti) const;
	
	bool CanStartGame() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
};
