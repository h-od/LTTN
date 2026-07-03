#include "TraversalComponent.h"

#include "AnimationWarpingLibrary.h"
#include "ChooserFunctionLibrary.h"
#include "PlayMontageCallbackProxy.h"
#include "Char0/Blueprint/Actor/Traversable/TraversableActor.h"
#include "Char0/Blueprint/Animation/LttnAnimInstance.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Data/Traversal/TraversalActionType.h"
#include "Char0/Blueprint/Data/Traversal/TraversalCheckInputs.h"
#include "Char0/Blueprint/Data/Traversal/TraversalChooserInputs.h"
#include "Char0/Blueprint/Data/Traversal/TraversalChooserOutputs.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PoseSearch/PoseSearchHistory.h"

UTraversalComponent::UTraversalComponent()
{
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
}

void UTraversalComponent::BeginPlay()
{
	Super::BeginPlay();
	Properties = GetLttnCharacter()->GetProperties_Traversal();
}

bool UTraversalComponent::TryTraversal(const FTraversalCheckInputs& Inputs)
{
	CacheValues();
	if (FindAndSetLedges(Inputs))
	{
		return true;
	}
	if (!CheckResult.bHasFrontLedge)
	{
		return true;
	}
	if (FrontLedgeHasRoom())
	{
		return true;
	}
	if (!FindBackLedgeAndSetDepth())
	{
		return true;
	}
	FindBackFloorAndSetDepth();
	//TODO should multicast HERE, server doesn't seem to know where chlients hand should be
	SendFrontLedgeLocationToAnim();

	// TODO should probably multicast here rather than sending Montage over network?
	ChooseMontage(GetLttnAnimInstance(), GetChooserInputs());

	if (CheckResult.ActionType == ETraversalActionType::None)
	{
		return false;
	}

	Server_PerformTraversal(CheckResult);

	return false;
}

void UTraversalComponent::CacheValues()
{
	const ALttnCharacter* Char = GetLttnCharacter();
	Properties = Char->GetProperties_Traversal();

	const UCapsuleComponent* CapsuleComponent = Properties.Capsule;
	ActorLocation = Char->GetActorLocation();
	CapsuleRadius = CapsuleComponent->GetUnscaledCapsuleRadius();
	CapsuleHalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();
	CheckResult = FTraversalCheckResult();
}

bool UTraversalComponent::FindAndSetLedges(const FTraversalCheckInputs& Input)
{
	// Find Traversable And Set Ledge Transforms
	const FVector Start = UKismetMathLibrary::Add_VectorVector(ActorLocation, Input.TraceOriginOffset);
	const FVector End = UKismetMathLibrary::Add_VectorVector(
		UKismetMathLibrary::Add_VectorVector(
			Start,
			UKismetMathLibrary::Multiply_VectorFloat(Input.TraceForwardDirection, Input.TraceForwardDistance)
		),
		Input.TraceEndOffset
	);

	FHitResult OutHitForTraversable;
	const TArray<AActor*> ActorsToIgnore;

	// Find Traversable Actor
	UKismetSystemLibrary::CapsuleTraceSingle(
		GetWorld(),
		Start,
		End,
		Input.TraceRadius,
		Input.TraceHalfHeight,
		TraversableTraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::None,
		OutHitForTraversable,
		true,
		FLinearColor::Transparent,
		FLinearColor::Transparent,
		0.0f
	);

	if (!OutHitForTraversable.bBlockingHit)
	{
		return true;
	}
	if (!OutHitForTraversable.GetActor()->IsA(ATraversableActor::StaticClass()))
	{
		return true;
	}

	//TODO check here if traversable is Valid for traversal
	// - can't be taller than 1m 
	// - can't be deeper than 1m? .8m? 
	ATraversableActor* Actor = Cast<ATraversableActor>(OutHitForTraversable.GetActor());
	CheckResult.HitComponent = OutHitForTraversable.GetComponent();
	Actor->GetLedges(
		OutHitForTraversable.ImpactPoint,
		ActorLocation,
		CheckResult
	);

	return false;
}

bool UTraversalComponent::FrontLedgeHasRoom()
{
	HasRoomCheckFrontLedgeLocation = UKismetMathLibrary::Add_VectorVector(
		UKismetMathLibrary::Add_VectorVector(
			CheckResult.FrontLedgeLocation,
			UKismetMathLibrary::Multiply_VectorFloat(CheckResult.FrontLedgeNormal, CapsuleRadius + 2.0f)
		),
		FVector(0.0, 0.0, CapsuleHalfHeight + 2.0f)
	);

	FHitResult OutHitHeight;
	TArray<AActor*> ActorsToIgnore;
	UKismetSystemLibrary::CapsuleTraceSingle(
		GetWorld(),
		ActorLocation,
		HasRoomCheckFrontLedgeLocation,
		CapsuleRadius,
		CapsuleHalfHeight,
		VisibilityTraceChannel,
		false,
		ActorsToIgnore, //todo null?
		EDrawDebugTrace::Type::None,
		OutHitHeight,
		true
		// FLinearColor::Green,
		// FLinearColor::Red,
		// 5.0f
	);

	if (OutHitHeight.bBlockingHit || OutHitHeight.bStartPenetrating)
	{
		CheckResult.bHasFrontLedge = false;
		return true;
	}
	// Save height of obstacle
	CheckResult.ObstacleHeight = UKismetMathLibrary::Abs(
		UKismetMathLibrary::Subtract_VectorVector(
			UKismetMathLibrary::Subtract_VectorVector(
				ActorLocation,
				FVector(0.0, 0.0, CapsuleHalfHeight)
			), CheckResult.FrontLedgeLocation
		).Z
	);
	return false;
}

bool UTraversalComponent::FindBackLedgeAndSetDepth()
{
	//Find Back Ledge And Set Depth
	HasRoomCheckBackLedgeLocation = UKismetMathLibrary::Add_VectorVector(
		UKismetMathLibrary::Add_VectorVector(
			CheckResult.BackLedgeLocation,
			UKismetMathLibrary::Multiply_VectorFloat(CheckResult.BackLedgeNormal, CapsuleRadius + 2.0f)
		),
		FVector(0.0, 0.0, CapsuleHalfHeight + 2.0f)
	);

	FHitResult OutHitSurface;
	TArray<AActor*> ActorsToIgnore;
	const bool bHadHit = UKismetSystemLibrary::CapsuleTraceSingle(
		GetWorld(),
		HasRoomCheckFrontLedgeLocation,
		HasRoomCheckBackLedgeLocation,
		CapsuleRadius,
		CapsuleHalfHeight,
		VisibilityTraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::None,
		OutHitSurface,
		true,
		FLinearColor::Transparent,
		FLinearColor::Transparent,
		0.0f
	);

	if (bHadHit)
	{
		CheckResult.ObstacleDepth = UKismetMathLibrary::VSizeXY(
			OutHitSurface.ImpactPoint - CheckResult.FrontLedgeLocation
		);
		CheckResult.bHasBackLedge = false;
		return false;
	}
	CheckResult.ObstacleDepth = UKismetMathLibrary::VSizeXY(
		UKismetMathLibrary::Subtract_VectorVector(CheckResult.FrontLedgeLocation, CheckResult.BackLedgeLocation)
	);
	return true;
}

void UTraversalComponent::FindBackFloorAndSetDepth()
{
	//Then find floor
	const FVector HasRoomCheckBackFloorLocation = UKismetMathLibrary::Subtract_VectorVector(
		UKismetMathLibrary::Add_VectorVector(
			CheckResult.BackLedgeLocation,
			UKismetMathLibrary::Multiply_VectorFloat(CheckResult.BackLedgeNormal, CapsuleRadius + 2.0f)
		),
		FVector(0.0, 0.0, 50.0)
	);

	FHitResult OutHitFloor;
	TArray<AActor*> ActorsToIgnore;
	UKismetSystemLibrary::CapsuleTraceSingle(
		GetWorld(),
		HasRoomCheckBackLedgeLocation,
		HasRoomCheckBackFloorLocation,
		CapsuleRadius,
		CapsuleHalfHeight,
		VisibilityTraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::None,
		OutHitFloor,
		true,
		FLinearColor::Transparent,
		FLinearColor::Transparent,
		0.0f
	);

	if (OutHitFloor.bBlockingHit)
	{
		CheckResult.BackFloorLocation = OutHitFloor.ImpactPoint;
		CheckResult.bHasBackFloor = true;
		CheckResult.BackLedgeHeight = UKismetMathLibrary::Abs(
			UKismetMathLibrary::Subtract_VectorVector(
				OutHitFloor.ImpactPoint,
				CheckResult.BackLedgeLocation
			).Z
		);
	}
	else
	{
		CheckResult.bHasBackFloor = false;
	}
}

void UTraversalComponent::SendFrontLedgeLocationToAnim()
{
	const FTransform Transform = FTransform(
		UKismetMathLibrary::MakeRotFromZ(CheckResult.FrontLedgeNormal),
		CheckResult.FrontLedgeLocation,
		FVector(1.0f, 1.0f, 1.0f)
	);

	GetLttnAnimInstance()->SetInteraction(Transform);
}

void UTraversalComponent::SetWarpTargets() const
{
	// Set FrontLedge Warp Target
	Properties.MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("FrontLedge"),
		CheckResult.FrontLedgeLocation + FVector(0.0, 0.0, 0.5),
		UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::NegateVector(CheckResult.FrontLedgeNormal))
	);

	// If hurdle or vault Set BackLedge Target
	const FName TargetBackLedge("BackLedge");
	const FName CurveName_DistanceFromLedge("Distance_From_Ledge");

	float FrontLedgeToBackLedgeDistance;
	if (CheckResult.ActionType == ETraversalActionType::Hurdle or CheckResult.ActionType == ETraversalActionType::Vault)
	{
		TArray<FMotionWarpingWindowData> OutWindows;
		UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(CheckResult.ChosenMontage, TargetBackLedge, OutWindows);
		if (!OutWindows.IsEmpty())
		{
			UAnimationWarpingLibrary::GetCurveValueFromAnimation(CheckResult.ChosenMontage, CurveName_DistanceFromLedge, OutWindows[0].EndTime, FrontLedgeToBackLedgeDistance);
			Properties.MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(TargetBackLedge, CheckResult.BackLedgeLocation, FRotator::ZeroRotator);
		}
		else
		{
			Properties.MotionWarping->RemoveWarpTarget(TargetBackLedge);
		}
	}
	else
	{
		Properties.MotionWarping->RemoveWarpTarget(TargetBackLedge);
	}

	// If Hurdle Set BackFloor Target
	const FName TargetBackFloor("BackFloor");
	if (CheckResult.ActionType == ETraversalActionType::Hurdle)
	{
		TArray<FMotionWarpingWindowData> OutWindows;
		UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(CheckResult.ChosenMontage, TargetBackFloor, OutWindows);

		if (!OutWindows.IsEmpty())
		{
			float FrontLedgeToBackFloorDistance;
			UAnimationWarpingLibrary::GetCurveValueFromAnimation(CheckResult.ChosenMontage, CurveName_DistanceFromLedge, OutWindows[0].EndTime, FrontLedgeToBackFloorDistance);
			FVector TargetLocation = UKismetMathLibrary::Add_VectorVector(
				CheckResult.BackLedgeLocation,
				UKismetMathLibrary::Multiply_VectorFloat(
					CheckResult.BackLedgeNormal,
					UKismetMathLibrary::Abs(UKismetMathLibrary::Subtract_DoubleDouble(FrontLedgeToBackLedgeDistance, FrontLedgeToBackFloorDistance))
				)
			);
			TargetLocation.Z = CheckResult.BackFloorLocation.Z;
			Properties.MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(TargetBackFloor, TargetLocation, FRotator::ZeroRotator);
		}
		else
		{
			Properties.MotionWarping->RemoveWarpTarget(TargetBackFloor);
		}
	}
	else
	{
		Properties.MotionWarping->RemoveWarpTarget(TargetBackFloor);
	}
}

void UTraversalComponent::IgnoreComponentWhenMoving(const bool bShouldIgnore) const
{
	Properties.Capsule->IgnoreComponentWhenMoving(CheckResult.HitComponent, bShouldIgnore);
}

void UTraversalComponent::SetMovementMode(const EMovementMode NewMode)
{
	GetLttnCharacter()->GetCharacterMovement()->SetMovementMode(NewMode, 0);
}

void UTraversalComponent::SetReplicationBehavior(const bool ClientAuthoritative)
{
	const ACharacter* Owner = GetLttnCharacter();
	Owner->GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = ClientAuthoritative;
	Owner->GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = ClientAuthoritative;
}

void UTraversalComponent::Server_PerformTraversal_Implementation(const FTraversalCheckResult& Result)
{
	Multicast_PerformTraversal(Result);
}

void UTraversalComponent::FinishTraversalAction()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "UTraversalComponent::FinishTraversalAction ", true, true, FLinearColor::Red, 5.0f);
	SetReplicationBehavior(false);
	// bRetriggerTimerActive = false;
}

void UTraversalComponent::Multicast_PerformTraversal_Implementation(const FTraversalCheckResult& Result)
{
	CheckResult = Result;
	// UKismetSystemLibrary::PrintString(GetWorld(), "UTraversalComponent::Multicast_PerformTraversal_Implementation \n" + CheckResult.ToString(), true, true, FLinearColor::Red, 25.0f);
	// UKismetSystemLibrary::PrintString(GetWorld(), "UTraversalComponent::Multicast_PerformTraversal_Implementation \n" + CheckResult.ToString(), true, true, FLinearColor::Red, 25.0f);
	PlayMontage(Properties.Mesh, CheckResult.ChosenMontage, CheckResult.PlayRate, CheckResult.StartTime);
	bDoingAction = true;
	SetWarpTargets();
	IgnoreComponentWhenMoving(true);
	SetMovementMode(MOVE_Flying);
	SetReplicationBehavior(true);
}

void UTraversalComponent::ChooseMontage(ULttnAnimInstance* AnimInstance, FTraversalChooserInputs ChooserInputs)
{
	const FInstancedStruct ChooserInstance = UChooserFunctionLibrary::MakeEvaluateChooser(MontageChooser);
	FChooserEvaluationContext ChooserEvaluationContext = UChooserFunctionLibrary::MakeChooserEvaluationContext();
	FTraversalChooserOutputs Outputs;

	ChooserEvaluationContext.AddObjectParam(AnimInstance);
	ChooserEvaluationContext.AddStructParam(ChooserInputs);
	ChooserEvaluationContext.AddStructParam(Outputs);

	UAnimMontage* Montage{
		Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateObjectChooserBase(ChooserEvaluationContext, ChooserInstance, UAnimMontage::StaticClass()))
	};

	CheckResult.ActionType = Outputs.ActionType;
	CheckResult.StartTime = Outputs.MontageStartTime;
	CheckResult.ChosenMontage = Montage;
	CheckResult.PlayRate = 1.0f;
}

FTraversalChooserInputs UTraversalComponent::GetChooserInputs()
{
	return FTraversalChooserInputs(
		CheckResult.ActionType,
		CheckResult.bHasFrontLedge,
		CheckResult.bHasBackLedge,
		CheckResult.bHasBackFloor,
		CheckResult.ObstacleHeight,
		CheckResult.ObstacleDepth,
		CheckResult.BackLedgeHeight,
		UKismetMathLibrary::Vector_Distance(CheckResult.FrontLedgeLocation, Properties.Mesh->GetComponentLocation()),
		Properties.MovementMode,
		Properties.Gait,
		Properties.Speed,
		GetLttnAnimInstance()->GetPoseHistoryReference()
	);
}

void UTraversalComponent::PlayMontage(USkeletalMeshComponent* Mesh, UAnimMontage* Montage, const float PlayRate, const float StartTime)
{
	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
		Mesh,
		Montage,
		PlayRate,
		StartTime
	);

	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &UTraversalComponent::MontageComplete);
	PlayMontageCallbackProxy->OnBlendOut.AddDynamic(this, &UTraversalComponent::MontageComplete);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &UTraversalComponent::MontageComplete);
}

void UTraversalComponent::MontageComplete(FName NotifyName)
{
	PlayMontageCallbackProxy->OnCompleted.RemoveDynamic(this, &UTraversalComponent::MontageComplete);
	PlayMontageCallbackProxy->OnBlendOut.RemoveDynamic(this, &UTraversalComponent::MontageComplete);
	PlayMontageCallbackProxy->OnInterrupted.RemoveDynamic(this, &UTraversalComponent::MontageComplete);

	bDoingAction = false;
	IgnoreComponentWhenMoving(false);
	SetMovementMode(ToMovementMode(CheckResult.ActionType));

	FLatentActionInfo LatentInfo;

	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "FinishTraversalAction";
	LatentInfo.UUID = 123;
	LatentInfo.Linkage = 1;

	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.2f, LatentInfo);

	// if (bRetriggerTimerActive)
	// {
	// 	GetWorld()->GetTimerManager().ClearTimer(RetriggerHandle);
	// }
	//
	// GetWorld()->GetTimerManager().SetTimer(RetriggerHandle, this, &UTraversalComponent::FinishTraversalAction, 0.2f, false, 0.0f);
	// bRetriggerTimerActive = true;
}

ALttnCharacter* UTraversalComponent::GetLttnCharacter()
{
	if (LttnCharacter == nullptr)
	{
		LttnCharacter = Cast<ALttnCharacter>(GetOwner());
	}
	return LttnCharacter;
}

ULttnAnimInstance* UTraversalComponent::GetLttnAnimInstance()
{
	if (!LttnAnimInstance)
	{
		LttnAnimInstance = Cast<ULttnAnimInstance>(Properties.Mesh->GetAnimInstance());
	}
	return LttnAnimInstance;
}

EMovementMode UTraversalComponent::ToMovementMode(const ETraversalActionType ActionType)
{
	if (ActionType == ETraversalActionType::Vault)
	{
		return MOVE_Falling;
	}
	return MOVE_Walking;
}
