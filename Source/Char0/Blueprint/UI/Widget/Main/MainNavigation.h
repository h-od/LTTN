#pragma once

#include "MainNavigation.generated.h"

UENUM(BlueprintType)
enum class EMainNavigation : uint8
{
	StartSolo UMETA(DisplayName = "StartSolo"),
	StartHost UMETA(DisplayName = "StartHost"),
	StartJoin UMETA(DisplayName = "StartJoin"),
	
	Host UMETA(DisplayName = "Host"),
	BackFromHost UMETA(DisplayName = "Host"),
	Join UMETA(DisplayName = "Join"),
	BackFromJoin UMETA(DisplayName = "Join"),
	Settings UMETA(DisplayName = "Settings")
};
