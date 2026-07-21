
#pragma once

#include "CoreMinimal.h"
#include "InteractableType.generated.h"

UENUM(BlueprintType, Category = "Enum of all interactions")
enum class EInteractableType : uint8
{
	StartGame UMETA(DisplayName = "StartGame"),
	StartLevel UMETA(DisplayName = "StartLevel"),
	Resupply UMETA(DisplayName = "Resupply"),
	//upgrades
	UpgradeWeapon UMETA(DisplayName = "UpgradeWeapon"),
	UpgradeHealth UMETA(DisplayName = "UpgradeHealth"),
	UpgradeStamina UMETA(DisplayName = "UpgradeStamina"),
	UpgradeReloadSpeed UMETA(DisplayName = "UpgradeReloadSpeed"),
	UpgradeProjectileCapacity UMETA(DisplayName = "UpgradeProjectileCapacity"),
	
	Revive UMETA(DisplayName = "Revive"),
	
	NoInteraction UMETA(DisplayName = "NoInteraction")
};
