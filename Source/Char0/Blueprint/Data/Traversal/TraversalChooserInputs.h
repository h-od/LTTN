#pragma once
#include "PoseSearch/PoseSearchHistory.h"
#include "Char0/Blueprint/Data/Movement/Gait.h"
#include "Char0/Blueprint/Data/Movement/MovementType.h"

#include "TraversalChooserInputs.generated.h"

enum class EGait : uint8;
enum class EMovementType : uint8;
enum class ETraversalActionType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FTraversalChooserInputs
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETraversalActionType ActionType = ETraversalActionType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasFrontLedge = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasBackLedge = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasBackFloor = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ObstacleHeight = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ObstacleDepth = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BackLedgeHeight = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DistanceToLedge = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EMovementType MovementType = EMovementType::OnGround;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGait Gait = EGait::Run;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPoseHistoryReference PoseHistory;

	FTraversalChooserInputs() = default;

	FTraversalChooserInputs(
		const ETraversalActionType At,
		const bool bHfl,
		const bool bHbl,
		const bool bHbf,
		const float Oh,
		const float Od,
		const float Blh,
		const float Dtl,
		const EMovementType Mt,
		const EGait G,
		const float S,
		const FPoseHistoryReference& Ph
	)
	{
		ActionType = At;
		bHasFrontLedge = bHfl;
		bHasBackLedge = bHbl;
		bHasBackFloor = bHbf;
		ObstacleHeight = Oh;
		ObstacleDepth = Od;
		BackLedgeHeight = Blh;
		DistanceToLedge = Dtl;
		MovementType = Mt;
		Gait = G;
		Speed = S;
		PoseHistory = Ph;
	}
};
