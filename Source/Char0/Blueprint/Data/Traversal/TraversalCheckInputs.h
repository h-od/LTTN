#pragma once

#include "TraversalCheckInputs.generated.h"

enum class ETraversalActionType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FTraversalCheckInputs
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraceForwardDirection = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceForwardDistance = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraceOriginOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraceEndOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceRadius = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceHalfHeight = 0.0f;

	FTraversalCheckInputs() = default;

	FTraversalCheckInputs(
		const FVector& Tfd,
		const float TfDist,
		const FVector& Too,
		const FVector& Teo,
		const float Tr,
		const float Thh
	)
	{
		TraceForwardDirection = Tfd;
		TraceForwardDistance = TfDist;
		TraceOriginOffset = Too;
		TraceEndOffset = Teo;
		TraceRadius = Tr;
		TraceHalfHeight = Thh;
	}
};
