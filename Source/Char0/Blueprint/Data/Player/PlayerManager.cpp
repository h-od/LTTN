#include "PlayerManager.h"

#include "Player/PlayerInfo.h"
#include "Weapon/WeaponInfo.h"

FPlayerManager::FPlayerManager() : CurrentScore(0), TotalScore(0), SpentScore(0), BotsDestroyed(0), Player(FPlayerInfo()), Weapon(FWeaponInfo())
{
}

FPlayerManager::FPlayerManager(
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
)
{
	TotalScore = 0;
	SpentScore = 0;
	BotsDestroyed = 0;
	CurrentScore = Score; // 232,500 - max can spend (not including resupply)
	Player = FPlayerInfo(
		Health,
		MaxHealth,
		HealthCooldown,
		HealthRegenRate,
		ReloadTime,
		Projectiles,
		MaxProjectiles,
		Stamina,
		MaxStamina,
		StaminaRegenRate
	);
	Weapon = FWeaponInfo(
		ProjectileCount,
		MaxWeaponProjectiles,
		Damage,
		FireRate,
		Penetration,
		Range
	);
}

bool FPlayerManager::CanUpgradeHealth() const
{
	if (HealthLevel < MaxLevel)
	{
		return CurrentScore >= UpgradeHealthCost();
	}
	return false;
}

bool FPlayerManager::CanUpgradeStamina() const
{
	if (StaminaLevel < MaxLevel)
	{
		return CurrentScore >= UpgradeStaminaCost();
	}
	return false;
}

bool FPlayerManager::CanUpgradeReloadSpeed() const
{
	if (ReloadSpeedLevel < MaxLevel)
	{
		return CurrentScore >= UpgradeReloadSpeedCost();
	}
	return false;
}

bool FPlayerManager::CanUpgradeWeapon() const
{
	if (WeaponLevel < MaxLevel)
	{
		return CurrentScore >= UpgradeWeaponCost();
	}
	return false;
}

bool FPlayerManager::CanResupply() const
{
	//TODO introduce cost
	return Player.Projectiles < Player.MaxProjectiles;
}

bool FPlayerManager::CanUpgradeProjectileCapacity() const
{
	if (ProjectileCapacityLevel < MaxLevel)
	{
		return CurrentScore >= UpgradeProjectileCapacityCost();
	}
	return false;
}

int32 FPlayerManager::UpgradeHealth()
{
	const int32 Cost = UpgradeHealthCost();
	CurrentScore -= Cost;
	SpentScore += Cost;

	Player.MaxHealth += 5;
	if (HealthLevel == 1 or HealthLevel == 2 or HealthLevel == 3)
	{
		Player.HealthRegenRate++;
	}
	if (HealthLevel == 2 or HealthLevel == 3 or HealthLevel == 4)
	{
		Player.HealthCooldown /= 2;
	}

	HealthLevel++;
	return Cost;
}

int32 FPlayerManager::UpgradeStamina()
{
	const int32 Cost = UpgradeStaminaCost();
	CurrentScore -= Cost;
	SpentScore += Cost;

	Player.MaxStamina += 20;

	if (StaminaLevel == 2 or StaminaLevel == 3 or StaminaLevel == 4)
	{
		Player.StaminaRegenRate++;
	}

	StaminaLevel++;
	return Cost;
}

int32 FPlayerManager::UpgradeReloadSpeed()
{
	int32 Cost = UpgradeReloadSpeedCost();
	CurrentScore -= Cost;
	SpentScore += Cost;

	ReloadSpeedLevel++;
	Player.ReloadTime -= 1;
	return Cost;
}

int32 FPlayerManager::UpgradeWeapon()
{
	// range 1-5
	//damage 1,3,5
	//fr 2,4,5: 1s,.5s,.25s, .1s
	//cap 1,3,5: 25, 50, 100, 250
	//pen 3,4,5 1 ,3,5,?

	const int32 Cost = UpgradeWeaponCost();
	CurrentScore -= Cost;
	SpentScore += Cost;

	switch (WeaponLevel)
	{
	case 0:
		Weapon.Range *= 1.5;
		Weapon.Damage *= 2;
		Weapon.MaxProjectiles *= 2;
		break;
	case 1:
		Weapon.Range *= 1.5;
		Weapon.Damage *= 2;
		Weapon.FireRate = 0.5f;
		break;
	case 2:
		Weapon.Range *= 1.5;
		Weapon.Damage *= 2;
		Weapon.MaxProjectiles *= 2;
		Weapon.Penetration = 3;
		break;
	case 3:
		Weapon.Range *= 1.5;
		Weapon.Damage *= 2;
		Weapon.FireRate = 0.25f;
		Weapon.Penetration = 5;
		break;
	case 4:
		Weapon.Range *= 1.5;
		Weapon.Damage *= 2;
		Weapon.FireRate = 0.1f;
		Weapon.MaxProjectiles *= 2;
		Weapon.Penetration = 10; //TODO?
		break;
	default:
		//TODO throw exception?
		;
	}

	WeaponLevel++;
	return Cost;
	// //Range
	// GameplayManager->Gameplay.Weapon.Range *= 1.5;
	// //Damage
	// GameplayManager->Gameplay.Weapon.Damage++;
	// //fire rate
	// GameplayManager->Gameplay.Weapon.FireRate++;
	// //proj cap
	// GameplayManager->Gameplay.Weapon.MaxProjectiles += 5;
	// Overlay->SetMaxProjectiles(GameplayManager->Gameplay.Weapon.MaxProjectiles);
	// //pen
	// GameplayManager->Gameplay.Weapon.Penetration++;
}

int32 FPlayerManager::UpgradeProjectileCapacity()
{
	int32 Cost = UpgradeProjectileCapacityCost();
	CurrentScore -= Cost;
	SpentScore += Cost;

	ProjectileCapacityLevel++;
	Player.MaxProjectiles *= 2;
	return Cost;
}

void FPlayerManager::Reload()
{
	const int32 RequiredProjectiles = Weapon.MaxProjectiles - Weapon.ProjectileCount;
	if (const int32 Remaining = Player.Projectiles - RequiredProjectiles; Remaining >= 0)
	{
		Player.Projectiles -= RequiredProjectiles;
		Weapon.ProjectileCount += RequiredProjectiles;
	}
	else
	{
		Weapon.ProjectileCount += Player.Projectiles;
		Player.Projectiles = 0;
	}
}

void FPlayerManager::ResupplyProjectiles()
{
	if (const int32 Required = Weapon.MaxProjectiles * 2; Required + Player.Projectiles <= Player.MaxProjectiles)
	{
		Player.Projectiles += Required;
	}
	else
	{
		Player.Projectiles = Player.MaxProjectiles;
	}
}

bool FPlayerManager::CanFire() const
{
	return Weapon.ProjectileCount > 0;
}

int32 FPlayerManager::DecrementProjectileCount()
{
	Weapon.ProjectileCount--;
	return Weapon.ProjectileCount;
}

bool FPlayerManager::ShouldReload() const
{
	return Weapon.ProjectileCount > 0;
}

bool FPlayerManager::CanReload() const
{
	return Weapon.ProjectileCount < Weapon.MaxProjectiles and Player.Projectiles > 0;
}

float FPlayerManager::UpdateStamina(const int Delta)
{
	return Player.Stamina += Delta;
}

void FPlayerManager::SetPlayerHealthMax()
{
	Player.Health = Player.MaxHealth;
}

float FPlayerManager::UpdatePlayerHealth(const int32 Delta)
{
	Player.Health += Delta;
	return Player.Health/ Player.MaxHealth;
}

float FPlayerManager::IncrementPlayerHealth()
{
	Player.Health += 1;
	return Player.Health/ Player.MaxHealth;
}

int32 FPlayerManager::AddScore(const int32 Delta)
{
	TotalScore += Delta;
	return CurrentScore += Delta;
}

int32 FPlayerManager::GetCostForLevel(const int32 Level) const
{
	switch (Level)
	{
	case 1:
		return Level1Cost;
	case 2:
		return Level2Cost;
	case 3:
		return Level3Cost;
	case 4:
		return Level4Cost;
	case 5:
		return Level5Cost;
	default:
		return -1; //todo?
	}
}

int32 FPlayerManager::UpgradeHealthCost() const
{
	return GetCostForLevel(HealthLevel + 1);
}

int32 FPlayerManager::UpgradeStaminaCost() const
{
	return GetCostForLevel(StaminaLevel + 1);
}

int32 FPlayerManager::UpgradeReloadSpeedCost() const
{
	return GetCostForLevel(ReloadSpeedLevel + 1);
}

int32 FPlayerManager::UpgradeWeaponCost() const
{
	return GetCostForLevel(WeaponLevel + 1);
}

int32 FPlayerManager::UpgradeProjectileCapacityCost() const
{
	return GetCostForLevel(ProjectileCapacityLevel + 1);
}
