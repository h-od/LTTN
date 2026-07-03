#pragma once

#include "Gait.generated.h"

UENUM(BlueprintType)
enum class EGait : uint8
{
	Run UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint")
};
