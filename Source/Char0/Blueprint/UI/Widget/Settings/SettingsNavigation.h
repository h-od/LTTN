#pragma once

#include "SettingsNavigation.generated.h"

UENUM(BlueprintType)
enum class ESettingsNavigation : uint8
{
	Audio UMETA(DisplayName = "StartSolo"),
	Controls UMETA(DisplayName = "StartHost"),
	Graphics UMETA(DisplayName = "StartJoin")
};
