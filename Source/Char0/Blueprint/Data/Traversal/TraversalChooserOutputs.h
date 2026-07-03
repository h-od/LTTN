#pragma once

#include "TraversalChooserOutputs.generated.h"

enum class ETraversalActionType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FTraversalChooserOutputs
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETraversalActionType ActionType = ETraversalActionType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MontageStartTime = 0.0f;

	FTraversalChooserOutputs() = default;

	FTraversalChooserOutputs(
		const ETraversalActionType At,
		const float Mst
	)
	{
		ActionType = At;
		MontageStartTime = Mst;
	}
};
