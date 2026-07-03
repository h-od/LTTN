#pragma once
#include <string>

#include "WeaponInfo.generated.h"

USTRUCT()
struct CHAR0_API FWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ProjectileCount;
	
	UPROPERTY()
	int32 MaxProjectiles;
	
	UPROPERTY()
	float Damage;
	
	UPROPERTY()
	float FireRate;
	
	UPROPERTY()
	int32 Penetration;
	
	UPROPERTY()
	float Range;

	FWeaponInfo() : ProjectileCount(0), MaxProjectiles(0), Damage(0), FireRate(0), Penetration(0), Range(0)
	{
	}

	FWeaponInfo(
		int32 PC,
		int32 M,
		float D,
		float F,
		int32 P,
		float R
	)
	{
		ProjectileCount = PC;
		MaxProjectiles = M;
		Damage = D;
		FireRate = F;
		Penetration = P;
		Range = R;
	}

	FString ToString() const
	{
		const FString ProjectileCountString = FString("ProjectileCount: ") + FString(std::to_string(ProjectileCount).c_str());
		const FString MaxProjectilesString = FString("MaxProjectiles: ") + FString(std::to_string(MaxProjectiles).c_str());
		const FString DamageString = FString("Damage: ") + FString(std::to_string(Damage).c_str());
		const FString FireRateString = FString("FireRate: ") + FString(std::to_string(FireRate).c_str());
		const FString PenetrationString = FString("Penetration: ") + FString(std::to_string(Penetration).c_str());
		const FString RangeString = FString("Range: ") + FString(std::to_string(Range).c_str());

		return "\t" + ProjectileCountString + "\n"
			+ "\t" + MaxProjectilesString + "\n"
			+ "\t" + DamageString + "\n"
			+ "\t" + FireRateString + "\n"
			+ "\t" + PenetrationString + "\n"
			+ "\t" + RangeString + "\n";
	}
};
