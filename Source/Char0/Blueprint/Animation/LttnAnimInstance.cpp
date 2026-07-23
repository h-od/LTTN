#include "LttnAnimInstance.h"

#include "AnimationWarpingLibrary.h"
#include "AnimationWarpingTypes.h"
#include "ChooserFunctionLibrary.h"
#include "Animation/AnimNodeReference.h"
#include "BlendStack/BlendStackAnimNodeLibrary.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Data/Movement/Gait.h"
#include "Char0/Blueprint/Data/Movement/MovementState.h"
#include "Char0/Blueprint/Data/Movement/RotationMode.h"
#include "PoseSearch/MotionMatchingAnimNodeLibrary.h"
#include "PoseSearch/PoseSearchDatabase.h"
#include "PoseSearch/PoseSearchHistory.h"
#include "PoseSearch/PoseSearchHistoryCollectorAnimNodeLibrary.h"

ULttnAnimInstance::ULttnAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	bUseMultiThreadedAnimationUpdate = true;
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;//todo
	
	HeavyLandSpeedThreshold = -500.0f;
	Properties = FProperties_Animation(
		FPlayerInputState(false, false),
		EMovementType::OnGround,
		ERotationMode::OrientToMovement,
		EGait::Run,
		FTransform(),
		FVector::Zero(),
		FVector::Zero(),
		0.0f,
		0.0f,
		FRotator::ZeroRotator,
		FRotator::ZeroRotator,
		false,
		FVector::Zero()
	);
	CharTransform = FTransform();
	CharTransform_LastFrame = FTransform();
	RootTransform = FTransform();
	Acceleration = FVector(0.0f, 0.0f, 0.0f);
	Acceleration_LastFrame = FVector(0.0f, 0.0f, 0.0f);
	AccelerationAmount = 0.0f;
	bHasVelocity = false;
	Velocity = FVector(0.0f, 0.0f, 0.0f);
	Velocity_LastFrame = FVector(0.0f, 0.0f, 0.0f);
	RelativeAcceleration = FVector(0.0f, 0.0f, 0.0f);
	VelocityAcceleration = FVector(0.0f, 0.0f, 0.0f);
	LastNonZeroVelocity = FVector(0.0f, 0.0f, 0.0f);
	Speed2D = 0.0f;
	InteractionTransform = FTransform();

	// states
	MovementMode = EMovementType::OnGround;
	MovementMode_LastFrame = EMovementType::OnGround;
	RotationMode = ERotationMode::OrientToMovement;
	RotationMode_LastFrame = ERotationMode::OrientToMovement;
	MovementState = EMovementState::Idle;
	MovementState_LastFrame = EMovementState::Moving;
	Gait = EGait::Run;
	Gait_LastFrame = EGait::Run;

	// Trajectory
	TrajectoryGenerationData_Idle = FPoseSearchTrajectoryData(
		0.0f,
		100.0f,
		0.0f,
		false,
		FRuntimeFloatCurve(),
		false,
		FRuntimeFloatCurve()
	);
	TrajectoryGenerationData_Moving = FPoseSearchTrajectoryData(
		0.0f,
		0.0f,
		0.0f,
		false,
		FRuntimeFloatCurve(),
		false,
		FRuntimeFloatCurve()
	);
	Trajectory = FTransformTrajectory();
	TrajectoryCollision = FPoseSearchTrajectory_WorldCollisionResults();
	PreviousDesiredControllerYaw = 0.0f;
	Trj_FutureVelocity = FVector(0.0f, 0.0f, 0.0f);

	// foot placement
	PlantSettingsDefault = FFootPlacementPlantSettings(
		1.0f,
		10.0f,
		EFootPlacementLockType::PivotAroundBall,
		20.0f,
		0.2f,
		60.0f,
		0.2f,
		0.5f,
		0.2f,
		0.0f,
		100.0f,
		0.75f,
		false,
		false
	);
	PlantSettingsStops = FFootPlacementPlantSettings(
		1.0f,
		10.0f,
		EFootPlacementLockType::PivotAroundBall,
		40.0f,
		0.75f,
		60.0f,
		0.2f,
		0.5f,
		0.2f,
		0.0f,
		100.0f,
		0.75f,
		false,
		false
	);
	InterpolationSettingsDefault = FFootPlacementInterpolationSettings(
		100.0f,
		1.0f,
		450.0f,
		1.0f,
		1000.0f,
		1.0f,
		1000.0f,
		1.0f,
		450.0f,
		1.0f,
		true
	);
	InterpolationSettingsStops = FFootPlacementInterpolationSettings(
		250.0f,
		1.0f,
		450.0f,
		1.0f,
		1000.0f,
		1.0f,
		1000.0f,
		1.0f,
		450.0f,
		1.0f,
		true,
		false,
		true
	);

	// Motion matching
	CurrentPoseDatabaseTags.Empty();

	// default
	bHasOwningActor = false;
}

void ULttnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void ULttnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	bHasOwningActor = TryGetPawnOwner() != nullptr;
	if (bHasOwningActor)
	{
		UpdateProperties_Animation();
	}
}

void ULttnAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (bHasOwningActor)
	{
		UpdateLogic();
	}
}

FTransform ULttnAnimInstance::GetProperty_ActorTransform_Implementation()
{
	return FTransform();
}

FVector ULttnAnimInstance::GetProperty_InputAcceleration_Implementation()
{
	return FVector();
}

float ULttnAnimInstance::GetProperty_CurrentMaxAcceleration_Implementation()
{
	return 0.0f;
}

float ULttnAnimInstance::GetProperty_CurrentMaxDeceleration_Implementation()
{
	return 0.0f;
}

FVector ULttnAnimInstance::GetProperty_Velocity_Implementation()
{
	return FVector();
}

float ULttnAnimInstance::GetProperty_DeltaSecs_Implementation()
{
	return 0.0f;
}

EMovementType ULttnAnimInstance::GetProperty_MovementType_Implementation()
{
	return EMovementType::OnGround;
}

ERotationMode ULttnAnimInstance::GetProperty_RotationMode_Implementation()
{
	return ERotationMode::OrientToMovement;
}

EGait ULttnAnimInstance::GetProperty_Gait_Implementation()
{
	return EGait::Run;
}

FRotator ULttnAnimInstance::GetProperty_OrientationIntent_Implementation()
{
	return FRotator();
}

bool ULttnAnimInstance::GetProperty_WantsToAim_Implementation()
{
	return false;
}

float ULttnAnimInstance::GetProperty_LandVelocityZ_Implementation()
{
	return 0.0f;
}

UAnimInstance* ULttnAnimInstance::GetProperty_AnimInstance_Implementation()
{
	return nullptr;
}

FProperties_Animation ULttnAnimInstance::GetProperties_Animation()
{
	UpdateProperties_Animation();
	return Properties;
}

FTransform ULttnAnimInstance::GetInteractionTransform()
{
	return InteractionTransform;
}

EGait ULttnAnimInstance::GetGait()
{
	return Gait;
}

FPoseHistoryReference ULttnAnimInstance::GetPoseHistoryReference()
{
	FPoseSearchHistoryCollectorAnimNodeReference PoseHistoryAnimNode;
	bool Result;

	UPoseSearchHistoryCollectorAnimNodeLibrary::ConvertToPoseHistoryNodePure(GetPoseHistory(), PoseHistoryAnimNode, Result);
	return UPoseSearchHistoryCollectorAnimNodeLibrary::GetPoseHistoryReference(PoseHistoryAnimNode);
}

FAnimNodeReference ULttnAnimInstance::GetOffSetRoot_Implementation()
{
	return FAnimNodeReference();
}

FAnimNodeReference ULttnAnimInstance::GetPoseHistory_Implementation()
{
	return FAnimNodeReference();
}

void ULttnAnimInstance::UpdateProperties_Animation()
{
	if (!LttnCharacter)
	{
		LttnCharacter = Cast<ALttnCharacter>(GetOwningActor());
	}

	Properties = LttnCharacter->GetProperties_Animation();
}

void ULttnAnimInstance::UpdateTrajectory()
{
	FTransformTrajectory TempTrajectory;

	UPoseSearchTrajectoryLibrary::PoseSearchGenerateTransformTrajectory(
		GetProperty_AnimInstance(),
		Speed2D > 0.0f ? TrajectoryGenerationData_Moving : TrajectoryGenerationData_Idle,
		GetProperty_DeltaSecs(),
		Trajectory,
		PreviousDesiredControllerYaw,
		TempTrajectory,
		-1.0f,
		30,
		0.1,
		15
	);
	FTransformTrajectory OtherTempTrajectory;
	FPoseSearchTrajectory_WorldCollisionResults CollisionResult;
	const TArray<AActor*> ActorsToIgnore;

	UPoseSearchTrajectoryLibrary::HandleTransformTrajectoryWorldCollisions(
		GetWorld(),
		this,
		TempTrajectory,
		true,
		0.01f,
		OtherTempTrajectory,
		CollisionResult,
		VisibilityTraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		true,
		150.0f,
		FLinearColor::Transparent,
		FLinearColor::Transparent,
		0.0f
	);

	TrajectoryCollision = CollisionResult;
	Trajectory = OtherTempTrajectory;

	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory,
		0.4f,
		0.5f,
		Trj_FutureVelocity,
		false
	);
}

void ULttnAnimInstance::UpdateEssentialValues()
{
	CharTransform_LastFrame = CharTransform;
	CharTransform = GetProperty_ActorTransform();

	const FTransform OffsetRootTransform = UAnimationWarpingLibrary::GetOffsetRootTransform(GetOffSetRoot());
	const FRotator Rotation = OffsetRootTransform.Rotator();
	const FRotator InRotation(Rotation.Pitch, Rotation.Yaw + 90.0f, Rotation.Roll);

	RootTransform = FTransform(InRotation, OffsetRootTransform.GetLocation(), FVector(1.0f, 1.0f, 1.0f));

	Acceleration_LastFrame = Acceleration;
	Acceleration = GetProperty_InputAcceleration();

	AccelerationAmount = UKismetMathLibrary::SafeDivide(UKismetMathLibrary::VSize(Acceleration), GetProperty_CurrentMaxAcceleration());

	Velocity_LastFrame = Velocity;
	Velocity = GetProperty_Velocity();

	Speed2D = UKismetMathLibrary::VSizeXY(Velocity);
	bHasVelocity = Speed2D > 5.0f;

	VelocityAcceleration = UKismetMathLibrary::Divide_VectorFloat(
		UKismetMathLibrary::Subtract_VectorVector(Velocity, Velocity_LastFrame),
		GetProperty_DeltaSecs() < 0.001f ? 0.001f : GetProperty_DeltaSecs()
	);

	RelativeAcceleration = UKismetMathLibrary::LessLess_VectorRotator(
		VelocityAcceleration,
		RootTransform.Rotator()
	);

	if (bHasVelocity)
	{
		LastNonZeroVelocity = Velocity;
	}
}

void ULttnAnimInstance::UpdateStates()
{
	MovementMode_LastFrame = MovementMode;
	MovementMode = GetProperty_MovementType();

	RotationMode_LastFrame = RotationMode;
	RotationMode = GetProperty_RotationMode();

	MovementState_LastFrame = MovementState;
	MovementState = IsMoving() ? EMovementState::Moving : EMovementState::Idle;

	Gait_LastFrame = Gait;
	Gait = GetProperty_Gait();
}

void ULttnAnimInstance::UpdateLogic()
{
	UpdateTrajectory();
	UpdateEssentialValues();
	UpdateStates();
}

bool ULttnAnimInstance::IsMoving() const
{
	return UKismetMathLibrary::NotEqual_VectorVector(Velocity, FVector::Zero(), 0.1f) and UKismetMathLibrary::NotEqual_VectorVector(Acceleration, FVector::Zero(), 0.0f);
}

bool ULttnAnimInstance::IsStarting() const
{
	return IsMoving() and Trj_FutureVelocity.Size2D() >= Velocity.Size2D() + 100.0f and !CurrentPoseDatabaseTags.Contains(FName("Pivots"));
}

bool ULttnAnimInstance::IsPivoting() const
{
	float RotationPivot;
	switch (RotationMode)
	{
	case ERotationMode::OrientToMovement:
	default: // todo should this be the default?
		RotationPivot = 45.0f;
		break;
	case ERotationMode::Aim:
		RotationPivot = 0.0f;
		break;
	}

	return (UKismetMathLibrary::Abs(GetTrajectoryTurnAngle()) >= RotationPivot) and IsMoving();
}

bool ULttnAnimInstance::ShouldTurnInPlace()
{
	return (UKismetMathLibrary::Abs(
			UKismetMathLibrary::NormalizedDeltaRotator(Properties.OrientationIntent, RootTransform.Rotator()).Yaw
		) >= 50.0f) and
		(GetProperty_WantsToAim() or
			(MovementState == EMovementState::Idle and MovementState_LastFrame == EMovementState::Moving));;
}

bool ULttnAnimInstance::ShouldSpinTransition() const
{
	const bool bCond = UKismetMathLibrary::Abs(
		UKismetMathLibrary::NormalizedDeltaRotator(CharTransform.Rotator(), RootTransform.Rotator()).Yaw
	) >= 130.0f;

	return bCond and (Speed2D >= 150.0f) and !CurrentPoseDatabaseTags.Contains(FName("Pivots"));
}

bool ULttnAnimInstance::JustTraversed() const
{
	return !IsSlotActive(FName("DefaultSlot")) and GetCurveValue(FName("MovingTraversal")) > 0.0f and UKismetMathLibrary::Abs(GetTrajectoryTurnAngle()) <= 50.0f;
}

bool ULttnAnimInstance::JustLanded_Light() const
{
	return Properties.JustLanded and UKismetMathLibrary::Abs(Properties.LandVelocity.Z) < UKismetMathLibrary::Abs(HeavyLandSpeedThreshold);
}

bool ULttnAnimInstance::JustLanded_Heavy() const
{
	return Properties.JustLanded and UKismetMathLibrary::Abs(Properties.LandVelocity.Z) >= UKismetMathLibrary::Abs(HeavyLandSpeedThreshold);
}

float ULttnAnimInstance::GetTrajectoryTurnAngle() const
{
	return UKismetMathLibrary::NormalizedDeltaRotator(
		UKismetMathLibrary::Conv_VectorToRotator(Acceleration),
		UKismetMathLibrary::Conv_VectorToRotator(Velocity)
	).Yaw;
}

EOffsetRootBoneMode ULttnAnimInstance::GetOffsetRootRotationMode() const
{
	if (IsSlotActive(FName("DefaultSlot")))
	{
		return EOffsetRootBoneMode::Release;
	}
	return EOffsetRootBoneMode::Accumulate;
}

EOffsetRootBoneMode ULttnAnimInstance::GetOffsetRootTranslationMode() const
{
	if (IsSlotActive(FName("DefaultSlot")))
	{
		return EOffsetRootBoneMode::Release;
	}
	if (MovementMode == EMovementType::InAir)
	{
		return EOffsetRootBoneMode::Release;
	}
	if (IsMoving())
	{
		return EOffsetRootBoneMode::Interpolate;
	}

	return EOffsetRootBoneMode::Release;
}

float ULttnAnimInstance::GetOffsetRootTranslationHalfLife() const
{
	if (MovementState == EMovementState::Moving)
	{
		return 0.3f;
	}
	return 0.1f;
}

bool ULttnAnimInstance::EnableAimOffset() const
{
	const bool bCond = UKismetMathLibrary::Abs(GetAimOffset().X) <= (MovementState == EMovementState::Moving ? 180.0f : 115.0f);
	return bCond and GetSlotMontageLocalWeight(FName("DefaultSlot")) < 0.5f;
}

FVector2D ULttnAnimInstance::GetAimOffset() const
{
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(Properties.AimingRotation, RootTransform.Rotator());
	const FVector Vector(Delta.Yaw, Delta.Pitch, 0.0);
	const FVector VLerp = UKismetMathLibrary::VLerp(Vector, FVector::ZeroVector, GetCurveValue(FName("Disable_AO")));
	return FVector2D(VLerp.X, VLerp.Y);
}

FVector ULttnAnimInstance::CalculateRelativeAcceleration()
{
	const float MaxAcceleration = GetProperty_CurrentMaxAcceleration();
	const float MaxDeceleration = GetProperty_CurrentMaxDeceleration();

	if (MaxAcceleration > 0.0f and MaxDeceleration > 0.0f)
	{
		float MaxAccDecel;

		if (UKismetMathLibrary::Dot_VectorVector(Acceleration, Velocity) > 0.0f)
		{
			MaxAccDecel = MaxDeceleration;
		}
		else
		{
			MaxAccDecel = MaxDeceleration;
		}

		const FVector ClampSizeMax = UKismetMathLibrary::Vector_ClampSizeMax(VelocityAcceleration, MaxAccDecel);
		const FVector Vector = UKismetMathLibrary::Divide_VectorFloat(ClampSizeMax, MaxAccDecel);
		return UKismetMathLibrary::LessLess_VectorRotator(Vector, CharTransform.Rotator());
	}
	return FVector::ZeroVector;
}

FVector2D ULttnAnimInstance::GetLeanAmount()
{
	return FVector2D(
		CalculateRelativeAcceleration().Y * UKismetMathLibrary::MapRangeClamped(
			Speed2D, 165.0f, 375.0f, 0.5f, 1.0f
		),
		0.0f
	);
}

bool ULttnAnimInstance::EnableSteering(const FAnimNodeReference Node) const
{
	return (MovementState == EMovementState::Moving or MovementMode == EMovementType::InAir)
		and UBlendStackAnimNodeLibrary::GetCurrentBlendStackAnimIsActive(Node);
}

FQuat ULttnAnimInstance::GetDesiredFacing() const
{
	FTransformTrajectorySample OutSample;
	UPoseSearchTrajectoryLibrary::GetTransformTrajectorySampleAtTime(Trajectory, 0.5f, OutSample, false);
	return OutSample.Facing;
}

FFootPlacementPlantSettings ULttnAnimInstance::GetFootPlacementPlantSettings() const
{
	if (CurrentPoseDatabaseTags.Contains(FName("Stops")))
	{
		return PlantSettingsStops;
	}
	return PlantSettingsDefault;
}

FFootPlacementInterpolationSettings ULttnAnimInstance::GetFootPlacementInterpolationSettings() const
{
	if (CurrentPoseDatabaseTags.Contains(FName("Stops")))
	{
		return InterpolationSettingsStops;
	}
	return InterpolationSettingsDefault;
}

void ULttnAnimInstance::UpdateMotionMatching(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	const FInstancedStruct ChooserInstance = UChooserFunctionLibrary::MakeEvaluateChooser(PoseChooser);
	FChooserEvaluationContext ChooserEvaluationContext = UChooserFunctionLibrary::MakeChooserEvaluationContext();

	ChooserEvaluationContext.AddObjectParam(this);

	TArray Output{
		UChooserFunctionLibrary::EvaluateObjectChooserBaseMulti(ChooserEvaluationContext, ChooserInstance, UPoseSearchDatabase::StaticClass())
	};
	TArray<UPoseSearchDatabase*> Databases;

	//TODO must be a better way than this:
	for (UObject* Object : Output)
	{
		Databases.Add(Cast<UPoseSearchDatabase>(Object));
	}
	FMotionMatchingAnimNodeReference MMNode;
	bool Result;
	UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNodePure(Node, MMNode, Result);
	UMotionMatchingAnimNodeLibrary::SetDatabasesToSearch(MMNode, Databases, GetMMInterruptMode());
}

void ULttnAnimInstance::UpdateMotionMatchingPost(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult MMNodeResult;

	const FMotionMatchingAnimNodeReference MMNode = UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNode(Node, MMNodeResult);

	if (MMNodeResult == EAnimNodeReferenceConversionResult::Succeeded)
	{
		FPoseSearchBlueprintResult Result;
		bool bIsResultValid;
		UMotionMatchingAnimNodeLibrary::GetMotionMatchingSearchResult(
			MMNode,
			Result,
			bIsResultValid
		);

		if (bIsResultValid)
		{
			CurrentPoseDatabaseTags = Result.SelectedDatabase->Tags;
		}
	}
}

float ULttnAnimInstance::GetMMBlendTime() const
{
	switch (MovementMode)
	{
	case EMovementType::OnGround:
		{
			if (MovementMode_LastFrame == EMovementType::OnGround)
			{
				return 0.5f;
			}
			return 0.2f;
		}
	case EMovementType::InAir:
		{
			if (Velocity.Z > 100.0f)
			{
				return 0.15f;
			}
			return 0.5f;
		}
	default:
		return 0.0f;
	}
}

EPoseSearchInterruptMode ULttnAnimInstance::GetMMInterruptMode() const
{
	const bool bMovementModeNotEq = MovementMode != MovementMode_LastFrame;
	const bool bMovementStateNotEq = MovementState != MovementState_LastFrame;
	const bool bGaitNotEq = Gait != Gait_LastFrame;
	const bool bIsMoving = MovementState == EMovementState::Moving;
	const bool bIsOnGround = MovementMode == EMovementType::OnGround;

	const bool bGaitChangedAndMoving = bGaitNotEq and bIsMoving;
	const bool bMovementStateNotEqOrIsMoving = bMovementStateNotEq or bGaitChangedAndMoving;
	const bool bMovementStateNotEqOrIsMovingAndIsOnGround = bMovementStateNotEqOrIsMoving and bIsOnGround;

	return bMovementModeNotEq or bMovementStateNotEqOrIsMovingAndIsOnGround
		       ? EPoseSearchInterruptMode::InterruptOnDatabaseChange
		       : EPoseSearchInterruptMode::DoNotInterrupt;
}

float ULttnAnimInstance::GetMMNotifyRecencyTimeOut() const
{
	switch (Gait)
	{
	case EGait::Run:
		return 0.2f;
	case EGait::Sprint:
		return 0.16f;
	}
	return 0.0f;
}
