#pragma once

#include "SoundSide.generated.h"

UENUM(BlueprintType)
enum class ESoundSide : uint8
{
	None UMETA(DisplayName = "None"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};
