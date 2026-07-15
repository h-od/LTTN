#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerInfo.h"
#include "Weapon/WeaponInfo.h"
#include "PlayerManager.generated.h"

USTRUCT()
struct CHAR0_API FPlayerManager
{
	GENERATED_BODY()

	FPlayerManager();
	FPlayerManager(
		const int32 Score,
		const float Health,
		const float MaxHealth,
		const float HealthCooldown,
		const float HealthRegenRate,
		const float ReloadTime,
		const int32 Projectiles,
		const int32 MaxProjectiles,
		const float Stamina,
		const float MaxStamina,
		const float StaminaRegenRate,
		const int32 ProjectileCount,
		const int32 MaxWeaponProjectiles,
		const float Damage,
		const float FireRate,
		const int32 Penetration,
		const float Range
	);

private:
	int32 Level1Cost = 1500;
	int32 Level2Cost = 3000;
	int32 Level3Cost = 6000;
	int32 Level4Cost = 12000;
	int32 Level5Cost = 24000;

	int32 MaxLevel = 5;
	int32 HealthLevel = 0;
	int32 StaminaLevel = 0;
	int32 ReloadSpeedLevel = 0;
	int32 WeaponLevel = 0;
	int32 ProjectileCapacityLevel = 0;

public:
	UPROPERTY()
	int32 CurrentScore;
	UPROPERTY()
	int32 TotalScore;
	UPROPERTY()
	int32 SpentScore;
	UPROPERTY()
	int32 BotsDestroyed;
	
	UPROPERTY()
	FPlayerInfo Player;

	UPROPERTY()
	FWeaponInfo Weapon;
	
	bool CanUpgradeHealth() const;
	bool CanUpgradeStamina() const;
	bool CanUpgradeReloadSpeed() const;
	bool CanUpgradeWeapon() const;
	bool CanResupply() const;
	bool CanUpgradeProjectileCapacity() const;
	
	int32 UpgradeHealth();
	int32 UpgradeStamina();
	int32 UpgradeReloadSpeed();
	int32 UpgradeWeapon();
	int32 UpgradeProjectileCapacity();
	
	void Reload();
	void ResupplyProjectiles();
	bool CanFire() const;
	int32 DecrementProjectileCount();
	bool ShouldReload() const;
	bool CanReload() const;
	float UpdateStamina(int Delta);
	void SetPlayerHealthMax();
	float UpdatePlayerHealth(int32 Delta);
	float IncrementPlayerHealth();
	int32 AddScore(int32 Delta);

private:
	int32 GetCostForLevel(int32 Level) const;
	int32 UpgradeHealthCost() const;
	int32 UpgradeStaminaCost() const;
	int32 UpgradeReloadSpeedCost() const;
	int32 UpgradeWeaponCost() const;
	int32 UpgradeProjectileCapacityCost() const;
};
