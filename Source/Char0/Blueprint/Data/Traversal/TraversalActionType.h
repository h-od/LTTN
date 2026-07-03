#pragma once

#include "TraversalActionType.generated.h"

UENUM(BlueprintType)
enum class ETraversalActionType : uint8
{
	None UMETA(DisplayName = "None"),
	Hurdle UMETA(DisplayName = "Hurdle"),
	Vault UMETA(DisplayName = "Vault")
};
