#include "LttnController.h"

#include "EnhancedInputSubsystems.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Character/Spectate/SpectateCharacter.h"
#include "Char0/Blueprint/GameMode/LttnGameMode.h"
#include "Char0/Blueprint/State/Game/LttnGameState.h"
#include "Char0/Blueprint/UI/HUD/LttnHud.h"
#include "Char0/Blueprint/UI/Widget/Gameplay/GameSummaryWidget.h"
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
	// 	if (IsLocalPlayerController())
	// 	{
	// 		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	// 		{
	// 			for (const UInputMappingContext* Context : MappingContexts)
	// 			{
	// 				Subsystem->AddMappingContext(Context, 0);
	// 			}
	// 		}
	// 	}
}

void ALttnController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
	// UKismetSystemLibrary::PrintString(GetWorld(), "OnPossess: " + PawnToPossess->GetName(), true, false, FLinearColor::White, 5.0f);
	LttnCharacter = Cast<ALttnCharacter>(PawnToPossess);
	if (LttnCharacter)
	{
		InitialiseHud(LttnCharacter->GetPlayerManager()); //TODO move this to Client_OnPossess
	}
	if (IsPaused())
	{
		SetPause(false);
	}
	Client_OnPossess(PawnToPossess->IsA(ASpectateCharacter::StaticClass()));
}

void ALttnController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALttnController, Id);
}

// void ALttnController::JoinGame()
// {
// 	Server_JoinGame();
// }

// void ALttnController::Server_JoinGame_Implementation()
// {
// 	GetLttnGameMode()->PlayerJoined(this);
// }

void ALttnController::ShowStartGame()
{
	Client_ShowStartGame();
}

void ALttnController::SetWave(const int32 Wave)
{
	Client_SetWave(Wave);
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

void ALttnController::ShowPlayerProjectiles(const int32 Projectiles) const
{
	HUD->SetPlayerProjectiles(Projectiles);
}

void ALttnController::ShowCanInteract(const bool bShow, const int32 Cost) const
{
	if (!IsLocalController())
	{
		return;
	}
	HUD->ShowInteract(bShow, Cost);
}

void ALttnController::StartGame()
{
	Server_StartGame();
}

void ALttnController::PlayerDead()
{
	Server_PlayerDead();
}

void ALttnController::Server_PlayerDead_Implementation()
{
	GetLttnGameMode()->PlayerDead(Id);

	// if (ALttnCharacter* Char = Cast<ALttnCharacter>(GetCharacter()))
	// {
	// 	Char->SetRagDoll();
	// }
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
	const FAttachmentTransformRules Rules = FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		true
	);

	SpectatePawn->AttachToActor(CharacterToSpectate, Rules, "head");
	Client_ShowSpectate();
}

void ALttnController::SpectatePrevious()
{
	Server_SpectatePrevious();
}

void ALttnController::SpectateNext()
{
	Server_SpectateNext();
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

void ALttnController::EnableSphere() const
{
	if (ALttnCharacter* Char = LttnCharacter)
	{
		Char->EnableCollisionSphere(true);
	}
}

void ALttnController::DisableSphere() const
{
	if (ALttnCharacter* Char = LttnCharacter)
	{
		Char->EnableCollisionSphere(false);
	}
}

void ALttnController::OpenDoor(const int32 DoorNumber)
{
	Server_OpenDoor(DoorNumber);
}

bool ALttnController::DoPause()
{
	//TODO if its single player then pause proper, if not then inform the UI
	if (GameState and GameState->PlayerArray.Num() == 1)
	{
		// Pause();
		SetPause(true);
		return true;
	}

	return false;
}

void ALttnController::Client_OnPossess_Implementation(const bool bIsSpectate)
{
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (bIsSpectate)
			{
				Subsystem->RemoveMappingContext(MappingContext);
				Subsystem->AddMappingContext(SpectateMappingContext, 0);
			}
			else
			{
				Subsystem->AddMappingContext(MappingContext, 0);
				Subsystem->RemoveMappingContext(SpectateMappingContext);
			}
		}
	}
}

void ALttnController::Server_OpenDoor_Implementation(int32 DoorNumber)
{
	GetLttnGameMode()->OpenDoor(DoorNumber);
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
	HUD->ShowSpectating(false);
}

void ALttnController::Server_StartGame_Implementation()
{
	GetLttnGameMode()->StartGame();
}

void ALttnController::Client_ShowStartGame_Implementation()
{
	// HUD->ShowGameStart();
}

void ALttnController::Client_ShowSpectate_Implementation()
{
	HUD->ShowSpectating(true);
}

void ALttnController::Server_SpectatePrevious_Implementation()
{
	if (!SpectatePawn)
	{
		return; //TODO should probably spawn?
	}

	const FAttachmentTransformRules Rules = FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		true
	);

	CurrentlySpectating = GetLttnGameMode()->GetPreviousPawnToSpectate(CurrentlySpectating);
	SpectatePawn->AttachToActor(GetLttnGameMode()->GetPlayerPawn(CurrentlySpectating), Rules, "head");
}

void ALttnController::Server_SpectateNext_Implementation()
{
	if (!SpectatePawn)
	{
		return; //TODO should probably spawn?
	}

	const FAttachmentTransformRules Rules = FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		true
	);

	CurrentlySpectating = GetLttnGameMode()->GetNextPawnToSpectate(CurrentlySpectating);
	SpectatePawn->AttachToActor(GetLttnGameMode()->GetPlayerPawn(CurrentlySpectating), Rules, "head");
}

void ALttnController::Client_SetWave_Implementation(const int32 Wave)
{
	// UKismetSystemLibrary::PrintString(GetWorld(), "Start wave: " + FString::FromInt(Wave), true, false, FLinearColor::White, 5.0f);
	HUD->SetWave(Wave);
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
		LttnGameMode = Cast<ALttnGameMode>(GetWorld()->GetAuthGameMode());
	}

	return LttnGameMode;
}
