#pragma once

#include "CameraMode.generated.h"

UENUM(BlueprintType)
enum class  ECameraMode : uint8
{
	Free UMETA(DisplayName = "Free"),
	Aim UMETA(DisplayName = "Aim")
};
