#pragma once

#include "Char0/Blueprint/Data/Traversal/TraversalActionType.h"
#include "ChooserOutputs.generated.h"

enum class ETraversalActionType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FChooserOutputs
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETraversalActionType ActionType = ETraversalActionType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MontageStateTime = 0.0f;

	FChooserOutputs() = default;

	FChooserOutputs(
		const ETraversalActionType At,
		const float Mt
	)
	{
		ActionType = At;
		MontageStateTime = Mt;
	}
};
