#include "LttnController.h"

#include "EnhancedInputSubsystems.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/GameMode/LttnGameMode.h"
#include "Char0/Blueprint/UI/HUD/LttnHud.h"
#include "Net/UnrealNetwork.h"

void ALttnController::BeginPlay()
{
	Super::BeginPlay();
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

void ALttnController::GameOver(bool bIsMulti)
{
	Client_GameOver(bIsMulti);
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

void ALttnController::Client_GameOver_Implementation(bool bIsMulti)
{
	//todo add fade to black then show stats
}

ALttnGameMode* ALttnController::GetLttnGameMode()
{
	if (!LttnGameMode)
	{
		LttnGameMode =Cast<ALttnGameMode>(GetWorld()->GetAuthGameMode());
	}
	
	return LttnGameMode;
}
