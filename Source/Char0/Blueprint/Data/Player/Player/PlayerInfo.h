#pragma once

#include <string>

#include "CoreMinimal.h"
#include "PlayerInfo.generated.h"

USTRUCT()
struct CHAR0_API FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;
	UPROPERTY()
	float HealthCooldown;
	UPROPERTY()
	float HealthRegenRate;

	UPROPERTY()
	float ReloadTime;
	UPROPERTY()
	int32 Projectiles;
	UPROPERTY()
	int32 MaxProjectiles;

	UPROPERTY()
	float Stamina;
	UPROPERTY()
	float MaxStamina;
	UPROPERTY()
	float StaminaRegenRate;

	FPlayerInfo() : Health(-1.0f),
	                MaxHealth(-1.0f),
	                HealthCooldown(-1.0f),
	                HealthRegenRate(-1.0f),
	                ReloadTime(-1.0f),
	                Projectiles(-1.0f),
	                MaxProjectiles(-1.0f),
	                Stamina(-1.0f),
	                MaxStamina(-1.0f),
	                StaminaRegenRate(-1.0f)
	{
	}

	FPlayerInfo(
		float H,
		float MH,
		float HC,
		float HR,
		float RT,
		int32 P,
		int32 MP,
		float S,
		float MS,
		float SRR
	)
	{
		Health = H;
		MaxHealth = MH;
		HealthCooldown = HC;
		HealthRegenRate = HR;
		ReloadTime = RT;
		Projectiles = P;
		MaxProjectiles = MP;
		Stamina = S;
		MaxStamina = MS;
		StaminaRegenRate = SRR;
	}

	FString ToString() const
	{
		const FString HealthString = FString("Health: ") + FString(std::to_string(Health).c_str());
		const FString MaxHealthString = FString("MaxHealth: ") + FString(std::to_string(MaxHealth).c_str());
		const FString HealthCooldownString = FString("HealthCooldown: ") + FString(
			std::to_string(HealthCooldown).c_str());
		const FString HealthRegenRateString = FString("HealthRegenRate: ") + FString(
			std::to_string(HealthRegenRate).c_str());
		const FString ReloadTimeString = FString("ReloadTime: ") + FString(std::to_string(ReloadTime).c_str());
		const FString ProjectilesString = FString("Projectiles: ") + FString(std::to_string(Projectiles).c_str());
		const FString MaxProjectilesString = FString("MaxProjectiles: ") + FString(
			std::to_string(MaxProjectiles).c_str());
		const FString StaminaString = FString("Stamina: ") + FString(std::to_string(Stamina).c_str());
		const FString MaxStaminaString = FString("MaxStamina: ") + FString(std::to_string(MaxStamina).c_str());
		const FString StaminaRegenRateString = FString("StaminaRegenRate: ") + FString(
			std::to_string(StaminaRegenRate).c_str());

		return "\t" + HealthString + "\n"
			+ "\t" + MaxHealthString + "\n"
			+ "\t" + HealthCooldownString + "\n"
			+ "\t" + HealthRegenRateString + "\n"
			+ "\t" + ReloadTimeString + "\n"
			+ "\t" + ProjectilesString + "\n"
			+ "\t" + MaxProjectilesString + "\n"
			+ "\t" + StaminaString + "\n"
			+ "\t" + MaxStaminaString + "\n"
			+ "\t" + StaminaRegenRateString + "\n";
	}
};
