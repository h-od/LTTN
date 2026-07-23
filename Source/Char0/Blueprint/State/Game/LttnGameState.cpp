#include "LttnGameState.h"

#include <string>

#include "Net/UnrealNetwork.h"

void ALttnGameState::BeginPlay()
{
	PlayerOne = FPlayerSummary(0);
	PlayerTwo = FPlayerSummary(1);
	PlayerThree = FPlayerSummary(2);
	Super::BeginPlay();
}

void ALttnGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALttnGameState, PlayerOne);
	DOREPLIFETIME(ALttnGameState, PlayerTwo);
	DOREPLIFETIME(ALttnGameState, PlayerThree);
}

void ALttnGameState::OnRep_PlayerOne()
{
	OnPlayerOneUpdated();
}

void ALttnGameState::OnRep_PlayerTwo()
{
	OnPlayerTwoUpdated();
}

void ALttnGameState::OnRep_PlayerThree()
{
	OnPlayerThreeUpdated();
}

void ALttnGameState::OnPlayerOneUpdated()
{
	UE_LOG(LogHAL, Log, TEXT("OnPlayerOneUpdated()"));
}

void ALttnGameState::OnPlayerTwoUpdated()
{
	UE_LOG(LogHAL, Log, TEXT("OnPlayerTwoUpdated()"));
}

void ALttnGameState::OnPlayerThreeUpdated()
{
	UE_LOG(LogHAL, Log, TEXT("OnPlayerThreeUpdated()"));
}

void ALttnGameState::RevivedPlayer(const int32 PlayerId)
{
	if (!HasAuthority())
	{
		return;
	}
	switch (PlayerId)
	{
	case 0:
		PlayerOne.Revives++;
		OnPlayerOneUpdated();
		break;
	case 1:
		PlayerTwo.Revives++;
		OnPlayerTwoUpdated();
		break;
	case 2:
		PlayerThree.Revives++;
		OnPlayerThreeUpdated();
		break;
	default:
		UE_LOG(LogHAL, Log, TEXT("RevivedPlayer not found? %d"), PlayerId);
	}
}

void ALttnGameState::PlayerDowned(const int32 PlayerId)
{
	if (!HasAuthority())
	{
		return;
	}
	switch (PlayerId)
	{
	case 0:
		PlayerOne.TimesDowned++;
		OnPlayerOneUpdated();
		break;
	case 1:
		PlayerTwo.TimesDowned++;
		OnPlayerTwoUpdated();
		break;
	case 2:
		PlayerThree.TimesDowned++;
		OnPlayerThreeUpdated();
		break;
	default:
		UE_LOG(LogHAL, Log, TEXT("PlayerDowned not found? %d"), PlayerId);
	}
}

void ALttnGameState::LevelCompleted()
{
	LevelsCompleted++;
}

void ALttnGameState::DecrementBot(const int32 PlayerId)
{
	if (!HasAuthority())
	{
		return;
	}
	switch (PlayerId)
	{
	case 0:
		PlayerOne.Destroyed++;
		OnPlayerOneUpdated();
		break;
	case 1:
		PlayerTwo.Destroyed++;
		OnPlayerTwoUpdated();
		break;
	case 2:
		PlayerThree.Destroyed++;
		OnPlayerThreeUpdated();
		break;
	default:
		UE_LOG(LogHAL, Log, TEXT("DecrementBot not found? %d"), PlayerId);
	}
}

FString ALttnGameState::GetSummary(const int32 PlayerId, const bool bIsMulti) const
{
	int32 Destroyed = 0;
	int32 TeamPointsEarned = 0;
	int32 TeamPointsSpent = 0;

	Destroyed += PlayerOne.Destroyed + PlayerTwo.Destroyed + PlayerThree.Destroyed;
	TeamPointsEarned += PlayerOne.PointsEarned + PlayerTwo.PointsEarned + PlayerThree.PointsEarned;
	TeamPointsSpent += PlayerOne.PointsSpent + PlayerTwo.PointsSpent + PlayerThree.PointsSpent;
	FPlayerSummary PlayerSummary;

	switch (PlayerId)
	{
	case 0: PlayerSummary = PlayerOne;
		break;
	case 1: PlayerSummary = PlayerTwo;
		break;
	case 2: PlayerSummary = PlayerThree;
		break;
	default: ;
	}

	FString Summary = "\n\t" + FString("Player: #") + FString(std::to_string(PlayerId).c_str()) +
		"\n\t" + FString("You survived ") + FString(std::to_string(LevelsCompleted).c_str()) + FString(" levels") +
		"\n\t" + FString("You destroyed ") + FString(std::to_string(PlayerSummary.Destroyed).c_str()) + FString(" enemies") +
		"\n\t" + FString("You earned ") + FString(std::to_string(PlayerSummary.PointsEarned).c_str()) + FString(" points") +
		"\n\t" + FString("You spent ") + FString(std::to_string(PlayerSummary.PointsSpent).c_str()) + FString(" points") +
		"\n\t" + FString("You died ") + FString(std::to_string(PlayerSummary.TimesDowned).c_str()) + FString(" times") +
		"\n\t" + FString("You resurrected ") + FString(std::to_string(PlayerSummary.Revives).c_str()) + FString(" players");

	if (bIsMulti)
	{
		Summary += "\n\n\t" + FString("Team destroyed ") + FString(std::to_string(Destroyed).c_str()) + FString(" enemies!") +
			"\n\t" + FString("Team earned  ") + FString(std::to_string(TeamPointsEarned).c_str()) + FString(" points!") +
			"\n\t" + FString("Team spent  ") + FString(std::to_string(TeamPointsSpent).c_str()) + FString(" points!");
	}

	return Summary;
}

bool ALttnGameState::CanStartGame() const
{
	return bGameStarted;
}

void ALttnGameState::ScoreIncreased(const int32 PlayerId, const int32 Amount)
{
	if (!HasAuthority())
	{
		return;
	}
	switch (PlayerId)
	{
	case 0:
		PlayerOne.PointsEarned += Amount;
		OnPlayerOneUpdated();
		break;
	case 1:
		PlayerTwo.PointsEarned += Amount;
		OnPlayerTwoUpdated();
		break;
	case 2:
		PlayerThree.PointsEarned += Amount;
		OnPlayerThreeUpdated();
		break;
	default:
		UE_LOG(LogHAL, Log, TEXT("ScoreIncreased not found? %d"), PlayerId);
	}
}

void ALttnGameState::ScoreDecreased(const int32 PlayerId, const int32 Amount)
{
	if (!HasAuthority())
	{
		return;
	}

	switch (PlayerId)
	{
	case 0:
		PlayerOne.PointsSpent += Amount;
		OnPlayerOneUpdated();
		break;
	case 1:
		PlayerTwo.PointsSpent += Amount;
		OnPlayerTwoUpdated();
		break;
	case 2:
		PlayerThree.PointsSpent += Amount;
		OnPlayerThreeUpdated();
		break;
	default:
		UE_LOG(LogHAL, Log, TEXT("ScoreDecreased not found? %d"), PlayerId);
	}
}
