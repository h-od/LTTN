#pragma once

#include "CameraStyle.generated.h"

UENUM(BlueprintType)
enum class ECameraStyle : uint8
{
	FirstP UMETA(DisplayName = "FirstPerson"),
	Close UMETA(DisplayName = "Close"),
	Medium UMETA(DisplayName = "Medium"),
	Far UMETA(DisplayName = "Far"),
	Debug UMETA(DisplayName = "Debug")
};
