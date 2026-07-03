#pragma once

#include "PlayerInputState.generated.h"

USTRUCT(BlueprintType)
struct CHAR0_API FPlayerInputState
{
	GENERATED_BODY()

	UPROPERTY()
	bool bWantsToSprint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bWantsToAim = false;
	
	FPlayerInputState() = default;

	FPlayerInputState(
		const bool bWts,
		const bool bWta
	)
	{
		bWantsToSprint = bWts;
		bWantsToAim = bWta;
	}
};
