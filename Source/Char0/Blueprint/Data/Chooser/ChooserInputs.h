#pragma once
#include "Char0/Blueprint/Data/Movement/Gait.h"
#include "Char0/Blueprint/Data/Movement/MovementType.h"
#include "Char0/Blueprint/Data/Traversal/TraversalActionType.h"
#include "PoseSearch/PoseSearchHistory.h"

#include "ChooserInputs.generated.h"

struct FPoseHistoryReference;
enum class EGait : uint8;
enum class EMovementType : uint8;
enum class ETraversalActionType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FChooserInputs
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	ETraversalActionType ActionType = ETraversalActionType::None;

	UPROPERTY(BlueprintReadOnly)
	bool HasFrontLedge = false;

	UPROPERTY(BlueprintReadOnly)
	bool HasBackLedge = false;

	UPROPERTY(BlueprintReadOnly)
	bool HasBackFloor = false;

	UPROPERTY(BlueprintReadOnly)
	float ObstacleHeight = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float ObstacleDepth =0.0f;

	UPROPERTY(BlueprintReadOnly)
	float BackLedgeHeight = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float DistanceToLedge = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	EMovementType MovementMode = EMovementType::OnGround;

	UPROPERTY(BlueprintReadOnly)
	EGait Gait = EGait::Run;

	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPoseHistoryReference PoseHistory;

	FChooserInputs() = default;
	
	FChooserInputs(
		const ETraversalActionType AT,
		const bool Hfl,
		const bool Hbl,
		const bool Hbf,
		const float Oh,
		const float Od,
		const float Blh,
		const float Dtl,
		const EMovementType Mm,
		const EGait G,
		const float S,
		const FPoseHistoryReference& Ph
	)
	{
		ActionType = AT;
		HasFrontLedge = Hfl;
		HasBackLedge = Hbl;
		HasBackFloor = Hbf;
		ObstacleHeight = Oh;
		ObstacleDepth = Od;
		BackLedgeHeight = Blh;
		DistanceToLedge = Dtl;
		MovementMode = Mm;
		Gait = G;
		Speed = S;
		PoseHistory = Ph;
	}
};
