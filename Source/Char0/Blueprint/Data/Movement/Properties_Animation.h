#pragma once

#include "Gait.h"
#include "PlayerInputState.h"
#include "MovementType.h"
#include "RotationMode.h"
#include "Properties_Animation.generated.h"

enum class ERotationMode : uint8;
enum class EMovementDirection : uint8;
class UMotionWarpingComponent;
class UCapsuleComponent;
enum class EGait : uint8;
enum class EMovementType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FProperties_Animation
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FPlayerInputState InputState;

	UPROPERTY(BlueprintReadOnly)
	EMovementType MovementType = EMovementType::OnGround;

	UPROPERTY(BlueprintReadOnly)
	ERotationMode RotationMode = ERotationMode::OrientToMovement;

	UPROPERTY(BlueprintReadOnly)
	EGait Gait = EGait::Run;

	UPROPERTY(BlueprintReadOnly)
	FTransform ActorTransform;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly)
	FVector InputAcceleration = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly)
	double CurrentMaxAcceleration = 0.0;

	UPROPERTY(BlueprintReadOnly)
	double CurrentMaxDeceleration = 0.0;

	UPROPERTY(BlueprintReadOnly)
	FRotator OrientationIntent = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly)
	FRotator AimingRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly)
	bool JustLanded = false;

	UPROPERTY(BlueprintReadOnly)
	FVector LandVelocity = FVector(0.0f, 0.0f, 0.0f);
	
	FProperties_Animation() = default;
	
	FProperties_Animation(
		FPlayerInputState Is,
		EMovementType Mo,
		ERotationMode Ro,
		EGait Ga,
		FTransform Ac,
		FVector Ve,
		FVector In,
		double Cma,
		double Cmd,
		FRotator Or,
		FRotator Ai,
		bool Ju,
		FVector La
	)
	{
		InputState = Is;
		MovementType = Mo;
		RotationMode = Ro;
		Gait = Ga;
		ActorTransform = Ac;
		Velocity = Ve;
		InputAcceleration = In;
		CurrentMaxAcceleration = Cma;
		CurrentMaxDeceleration = Cmd;
		OrientationIntent = Or;
		AimingRotation = Ai;
		JustLanded = Ju;
		LandVelocity = La;
	}
};
