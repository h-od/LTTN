#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Data/Player/PlayerManager.h"
#include "GameFramework/PlayerController.h"
#include "LttnController.generated.h"

class ALttnGameMode;
class ALttnHud;
class ALttnCharacter;
class UInputMappingContext;

UCLASS()
class CHAR0_API ALttnController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY()
	ALttnGameMode* LttnGameMode;
	UPROPERTY()
	ALttnCharacter* LttnCharacter;
	UPROPERTY()
	ALttnHud* HUD;
	UPROPERTY()
	ALttnCharacter*  ActorToDestroyOnRevive;
	
public:
	UPROPERTY(Replicated)
	int32 Id;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input Mappings")
	TArray<UInputMappingContext*> MappingContexts;
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* PawnToPossess) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void CantStartGame();
	void UpdateWeaponProjectiles(int32 Count) const;
	void PlayerIsReloading(float ReloadTime) const;
	void PlayerReloaded(int32 WeaponProjectileCount, int32 PlayerProjectileCount) const;
	void SetPlayerHealth(float Health) const;
	void SetPlayerStamina(float Stamina) const;
	void ShowScore(int32 CurrentScore, int32 Cost) const;
	void ShowMaxPlayerProjectiles(int32 MaxProjectiles) const;
	void ShowMaxWeaponProjectiles(int32 MaxProjectiles) const;
	void ShowResupplyCooldown(float TimerRemaining) const;
	void ShowPlayerProjectiles(int32 Projectiles) const;
	void ShowCanInteract(bool bShow) const;
	bool CanStartGame();
	bool CanStartLevel();
	void StartGame();
	void StartLevel();

	bool HasRagDoll() const;
	void DestroyRagdoll();
	void GameOver(bool bIsMulti);

private:
	void InitialiseHud(const FPlayerManager& PlayerManager);
	UFUNCTION(Client, Reliable)
	void Client_InitialiseHud(const FPlayerManager& PlayerManager);
	void Client_InitialiseHud_Implementation(const FPlayerManager& PlayerManager);
	
	UFUNCTION(Server, Reliable)
	void Server_StartGame();
	void Server_StartGame_Implementation();
	
	UFUNCTION(Server, Reliable)
	void Server_StartLevel();
	void Server_StartLevel_Implementation();
	
	UFUNCTION(Client, Reliable)
	void Client_CantStartGame();
	void Client_CantStartGame_Implementation();
	
	UFUNCTION(Client, Reliable)
	void Client_GameOver(bool bIsMulti);
	void Client_GameOver_Implementation(bool bIsMulti);
	
	ALttnGameMode* GetLttnGameMode();
};
