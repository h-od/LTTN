#pragma once

#include "MovementType.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	OnGround UMETA(DisplayName = "OnGround"),
	InAir UMETA(DisplayName = "InAir"),
	Traversing UMETA(DisplayName = "Traversing")
};
