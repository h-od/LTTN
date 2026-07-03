#pragma once

#include "MotionWarpingComponent.h"
#include "Char0/Blueprint/Data/Movement/Gait.h"
#include "Char0/Blueprint/Data/Movement/MovementType.h"
#include "Properties_Traversal.generated.h"

class UCapsuleComponent;
enum class EGait : uint8;
enum class EMovementType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FProperties_Traversal
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule; //todo don't need this? think we just need height/ radius. Are they always going to be the same?

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EMovementType MovementMode = EMovementType::OnGround;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGait Gait = EGait::Run;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double Speed = 0.0;
	
	FProperties_Traversal() = default;
	
	FProperties_Traversal(
		const TObjectPtr<UCapsuleComponent> C,
		const TObjectPtr<USkeletalMeshComponent> M,
		const TObjectPtr<UMotionWarpingComponent> Mwc,
		const EMovementType Mt,
		const EGait G,
		const double S
	)
	{
		Capsule = C;
		Mesh = M;
		MotionWarping = Mwc;
		MovementMode = Mt;
		Gait = G;
		Speed = S;
	}
};
