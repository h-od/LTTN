#include "LttnController.h"

#include "EnhancedInputSubsystems.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Character/Spectate/SpectateCharacter.h"
#include "Char0/Blueprint/GameMode/LttnGameMode.h"
#include "Char0/Blueprint/State/Game/LttnGameState.h"
#include "Char0/Blueprint/UI/HUD/LttnHud.h"
#include "Char0/Blueprint/UI/Widget/GameSummaryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ALttnController::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = Cast<ALttnGameState>(GetWorld()->GetGameState());
}

void ALttnController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (const UInputMappingContext* Context : MappingContexts)
			{
				Subsystem->AddMappingContext(Context, 0);
			}
		}
	}
}

void ALttnController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
	LttnCharacter = Cast<ALttnCharacter>(PawnToPossess);
	if (LttnCharacter)
	{
		InitialiseHud(LttnCharacter->GetPlayerManager());
	}
}

void ALttnController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALttnController, Id);
}

void ALttnController::CantStartGame()
{
	Client_CantStartGame();
}

void ALttnController::UpdateWeaponProjectiles(const int32 Count) const
{
	HUD->SetWeaponProjectiles(Count);
}

void ALttnController::PlayerIsReloading(const float ReloadTime) const
{
	HUD->ShowReloading(ReloadTime);
}

void ALttnController::PlayerReloaded(const int32 WeaponProjectileCount, const int32 PlayerProjectileCount) const
{
	HUD->SetWeaponProjectiles(WeaponProjectileCount);
	HUD->SetPlayerProjectiles(PlayerProjectileCount);
}

void ALttnController::SetPlayerHealth(const float Health) const
{
	Client_SetPlayerHealth(Health);
}

void ALttnController::Client_SetPlayerHealth_Implementation(const float Health) const
{
	HUD->SetHealth(Health);
}

void ALttnController::SetPlayerStamina(const float Stamina) const
{
	HUD->SetStamina(Stamina);
}

void ALttnController::ShowScore(int32 CurrentScore, int32 Cost) const
{
	HUD->SetScore(CurrentScore);
}

void ALttnController::ShowMaxPlayerProjectiles(const int32 MaxProjectiles) const
{
	HUD->SetMaxPlayerProjectiles(MaxProjectiles);
}

void ALttnController::ShowMaxWeaponProjectiles(const int32 MaxProjectiles) const
{
	HUD->SetMaxWeaponProjectiles(MaxProjectiles);
}

void ALttnController::ShowResupplyCooldown(const float TimerRemaining) const
{
	HUD->SetResupplyCooldown(TimerRemaining);
}

void ALttnController::ShowPlayerProjectiles(const int32 Projectiles) const
{
	HUD->SetPlayerProjectiles(Projectiles);
}

void ALttnController::ShowCanInteract(const bool bShow) const
{
	HUD->ShowInteract(bShow);
}

bool ALttnController::CanStartGame()
{
	return true; //TODO
}

bool ALttnController::CanStartLevel()
{
	return true; //todo
}

void ALttnController::StartGame()
{
	Server_StartGame();
}

void ALttnController::StartLevel()
{
	Server_StartLevel();
}

void ALttnController::PlayerDead()
{
	Server_PlayerDead();
}

void ALttnController::Server_PlayerDead_Implementation()
{
	GetLttnGameMode()->PlayerDead(Id);
}

void ALttnController::StartSpectate(const int32 SpectateId)
{
	if (bIsSpectating)
	{
		return;
	}
	bIsSpectating = true;
	ALttnCharacter* Char = Cast<ALttnCharacter>(GetCharacter());
	Char->SetRagDoll();
	ActorToDestroyOnRevive = Char;
	UnPossess();
	CurrentlySpectating = SpectateId;
	APawn* CharacterToSpectate = GetLttnGameMode()->GetPlayerPawn(SpectateId);
	
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	SpectatePawn = GetWorld()->SpawnActor<ASpectateCharacter>(SpectatorClass, CharacterToSpectate->GetActorLocation(), CharacterToSpectate->GetControlRotation(), SpawnParams);
	Possess(SpectatePawn);
	SpectatePawn->Attach(CharacterToSpectate);
}

bool ALttnController::HasRagDoll() const
{
	return ActorToDestroyOnRevive != nullptr;
}

void ALttnController::DestroyRagdoll()
{
	if (ActorToDestroyOnRevive)
	{
		ActorToDestroyOnRevive->Destroy();
		ActorToDestroyOnRevive = nullptr;
	}
}

void ALttnController::GameOver(const bool bIsMulti)
{
	Client_GameOver(bIsMulti);
}

FPlayerManager ALttnController::GetPlayerManager() const
{
	return ActorToDestroyOnRevive->GetPlayerManager();
}

void ALttnController::DoRevive(const int32 IdToRevive)
{
	Server_DoRevive(IdToRevive);
}

void ALttnController::EnableSphere()
{
	
	LttnCharacter->EnableCollisionSphere(true);
}

void ALttnController::DisableSphere()
{
	
	LttnCharacter->EnableCollisionSphere(false);
}

void ALttnController::InitialiseHud(const FPlayerManager& PlayerManager)
{
	Client_InitialiseHud(PlayerManager);
}

void ALttnController::Client_InitialiseHud_Implementation(const FPlayerManager& PlayerManager)
{
	if (!HUD)
	{
		HUD = Cast<ALttnHud>(GetHUD());
	}
	HUD->Initialise(PlayerManager);
}

void ALttnController::Server_StartGame_Implementation()
{
	GetLttnGameMode()->StartGame();
}

void ALttnController::Server_StartLevel_Implementation()
{
	GetLttnGameMode()->StartLevel();
}

void ALttnController::Client_CantStartGame_Implementation()
{
	HUD->ShowCantStartGame();
}

void ALttnController::Server_DoRevive_Implementation(const int32 IdToRevive)
{
	GetLttnGameMode()->RevivePlayer(Id, IdToRevive);
}

void ALttnController::Client_GameOver_Implementation(const bool bIsMulti)
{
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	UGameSummaryWidget* SummaryWidget = Cast<UGameSummaryWidget>(CreateWidget(GetWorld(), SummaryWidgetClass));

	SummaryWidget->AddToViewport();
	SummaryWidget->ShowSummary(
		FText::FromString(GameState->GetSummary(Id, bIsMulti)) 
	);
}

ALttnGameMode* ALttnController::GetLttnGameMode()
{
	if (!LttnGameMode)
	{
		LttnGameMode =Cast<ALttnGameMode>(GetWorld()->GetAuthGameMode());
	}
	
	return LttnGameMode;
}
