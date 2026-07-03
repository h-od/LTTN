#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Data/Gameplay/GameplayManager.h"
#include "Char0/Blueprint/Data/Player/PlayerManager.h"
#include "Components/ActorComponent.h"
#include "GameplayComponent.generated.h"

class UNiagaraSystem;
class ALttnCharacter;
struct FPlayerManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDepletedDelegate);

UCLASS()
class CHAR0_API UGameplayComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(Replicated)
	FPlayerManager PlayerManager;
	
	UPROPERTY()
	ALttnCharacter* Character;

	UPROPERTY()
	int32 CurrentHealth = 0;

	UPROPERTY()
	int32 BotDamage = 0;
	
	UPROPERTY()
	bool bFireRateCooldownActive = false;
	UPROPERTY()
	bool bShouldStopFiring = false;
	UPROPERTY()
	bool bIsReloading = false;
	UPROPERTY()
	bool bIsSprinting = false;
	UPROPERTY()
	bool bIsMoving = false;
	
	FStaminaDepletedDelegate StaminaDepletedDelegate;
	
	UPROPERTY()
	FTimerHandle HealthCooldownTimerHandle;
	UPROPERTY()
	FTimerHandle HealthRegenTimerHandle;
	UPROPERTY()
	FTimerHandle StaminaRegenTimerHandle; //Todo just one stamina timer handle?
	UPROPERTY()
	FTimerHandle StaminaDegenTimerHandle;
	UPROPERTY()
	FTimerHandle ReloadTimerHandle;
	UPROPERTY()
	FTimerHandle ResupplyTimerHandle;
	UPROPERTY()
	FTimerHandle FireRateTimerHandle;

protected:
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FireSystem;
	
	//CONFIG stuff - todo move to gamemode
	UPROPERTY(EditDefaultsOnly, Category="Config")
	int32 StartingScore;//0
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float Health;//10
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float MaxHealth;//10
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float HealthCooldown;//5
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float HealthRegenRate;//1
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float ReloadTime;//6
	UPROPERTY(EditDefaultsOnly, Category="Config")
	int32 Projectiles;//50
	UPROPERTY(EditDefaultsOnly, Category="Config")
	int32 MaxProjectiles;//100
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float Stamina;//100
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float MaxStamina;//100
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float StaminaRegenRate;//.1
	UPROPERTY(EditDefaultsOnly, Category="Config")
	int32 ProjectileCount;//25
	UPROPERTY(EditDefaultsOnly, Category="Config")
	int32 MaxWeaponProjectiles;//25
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float WeaponDamage;//1
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float FireRate;//1 TODO : maybe should be a bit lower?
	UPROPERTY(EditDefaultsOnly, Category="Config")
	int32 Penetration;//1
	UPROPERTY(EditDefaultsOnly, Category="Config")
	float Range;//1000 TODO : maybe should be a bit higher?
	//end CONFIG stuff
	
public:
	UGameplayComponent();

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FPlayerManager GetPlayerManager() const;
	
	void SetNewPlayerInfo();
	
	void LevelStarted();
	void SetMaxHealth();
	void TakeDamage(const FVector& DamageDirection);
	void StartFiring();
	void StopFiring();
	void Reload();
	void SetIsMoving(bool bMoving);
	void StartSprinting();//todo need a Delegate to broadcast when stamin runs out
	UFUNCTION()
	void StopSprinting();
	
	//upgrade
	bool CanUpgradeHealth() const;
	bool CanUpgradeStamina() const;
	bool CanUpgradeReloadSpeed() const;
	bool CanUpgradeWeapon() const;
	bool CanUpgradeProjectileCapacity() const;
	
	void UpgradeHealth();
	void UpgradeStamina();
	void UpgradeReloadSpeed();
	void UpgradeWeapon();
	void UpgradeProjectileCapacity();
	
	//resupply
	bool CanResupply() const; //todo returns cooldown 
	void Resupply();

private:
	ALttnCharacter* GetCharacter();
	
	void BeginHealthRegen();
	void IncrementHealth();
	
	//Fire
	void StartedFiring();
	void TryFire();
	void Fire();
	static TArray<FHitResult> SortHits(TArray<FHitResult> Array, int32 Penetration);
	void DidFire(const FVector& HitLocation);
	
	UFUNCTION(Server, Reliable)
	void Server_DidFire(const FVector& WeaponLocation, const FVector& HitLocation);
	void Server_DidFire_Implementation(const FVector& WeaponLocation, const FVector& HitLocation);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_DidFire(const FVector& WeaponLocation, const FVector& HitLocation);
	void Multi_DidFire_Implementation(const FVector& WeaponLocation, const FVector& HitLocation);
	//Reload
	void ReloadComplete();
	
	//Stamina
	void DecrementStamina();
	void IncrementStamina();
};
