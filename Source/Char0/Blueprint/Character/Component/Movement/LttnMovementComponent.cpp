#include "LttnMovementComponent.h"

ULttnMovementComponent::ULttnMovementComponent()
{
	SetIsReplicatedByDefault(true);
	GravityScale = 1.0f;
	MaxAcceleration = 500.0f;
	BrakingFrictionFactor = 0.0f;
	 CrouchedHalfHeight = 60.0f; //todo deprecated -> needed?
	bUseSeparateBrakingFriction = false;
	Mass = 100.0f;
	DefaultLandMovementMode = MOVE_Walking;
	bBasedMovementIgnorePhysicsBase = false;
	bBaseOnAttachmentRoot = false;
	BrakingSubStepTime = 0.030303f;
	bEnableScopedMovementUpdates = true;
	bEnableServerDualMoveScopedMovementUpdates = false;
	bRunPhysicsWithNoController = false;
	MaxSimulationTimeStep = 0.05f;
	MaxSimulationIterations = 8.0f;
	MaxJumpApexAttemptsPerSimulation = 2.0f;
	MaxDepenetrationWithGeometry = 500.0f;
	MaxDepenetrationWithGeometryAsProxy = 100.0f;
	MaxDepenetrationWithPawn = 100.0f;
	MaxDepenetrationWithPawnAsProxy = 2.0f;
	bRequestedMoveUseAcceleration = true;
	//CharacterMovement- walking
	MaxStepHeight = 45.0f;
	SetWalkableFloorAngle(44.765083f);
	GroundFriction = 8.0f;
	MaxWalkSpeed = 500.0f;
	MaxWalkSpeedCrouched = 300.0f;
	MinAnalogWalkSpeed = 150.0f;
	BrakingDecelerationWalking = 500.0f;
	bSweepWhileNavWalking = true;
	bCanWalkOffLedges = true;
	bCanWalkOffLedgesWhenCrouching = true;
	bMaintainHorizontalGroundVelocity = true;
	bIgnoreBaseRotation = false;
	PerchRadiusThreshold = 20.0f;
	PerchAdditionalHeight = 40.0f;
	LedgeCheckThreshold = 4.0f;
	bAlwaysCheckFloor = true;
	bUseFlatBaseForFloorChecks = true;
	//CharacterMovement- Jumping/Falling
	JumpZVelocity = 500.0;
	BrakingDecelerationFalling = 0.0f;
	AirControl = 0.25;
	AirControlBoostMultiplier = 2.0;
	AirControlBoostVelocityThreshold = 25.0;
	FallingLateralFriction = 0.0f;
	bImpartBaseVelocityX = true;
	bImpartBaseVelocityY = true;
	bImpartBaseVelocityZ = true;
	bImpartBaseAngularVelocity = true;
	bStayBasedInAir = false;
	JumpOffJumpZFactor = 0.5f;
	bDontFallBelowJumpZVelocityDuringJump = true;
	bApplyGravityWhileJumping = true;
	//Component Tick
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = false;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval = 0.0f;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//CharacterMovement- networking
	NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	bNetworkSkipProxyPredictionOnNetUpdate = false;
	NetworkMaxSmoothUpdateDistance = 256.0f;
	NetworkNoSmoothUpdateDistance = 384.0f;
	NetworkMinTimeBetweenClientAckGoodMoves = 0.1f;
	NetworkMinTimeBetweenClientAdjustments = 0.1f;
	NetworkMinTimeBetweenClientAdjustmentsLargeCorrection = 0.05f;
	NetworkLargeClientCorrectionDistance = 15.0f;
	bNetworkAlwaysReplicateTransformUpdateTimestamp = false;
	NetworkSimulatedSmoothLocationTime = 0.1f;
	NetworkSimulatedSmoothRotationTime = 0.05f;
	ListenServerNetworkSimulatedSmoothLocationTime = 0.04f;
	ListenServerNetworkSimulatedSmoothRotationTime = 0.033f;
	NetProxyShrinkRadius = 0.01f;
	NetProxyShrinkHalfHeight = 0.01f;
	//CharacterMovement- CustomMovement
	MaxCustomMovementSpeed = 600.0f;
	//CharacterMovement- Rotation Settings
	RotationRate = FRotator(0.0f, -1.0f, 0.0f);
	bUseControllerDesiredRotation = true;
	bOrientRotationToMovement = false;
	//CharacterMovement- Physics Interaction
	bEnablePhysicsInteraction = true;
	bTouchForceScaledToMass = true;
	bPushForceScaledToMass = false;
	bPushForceUsingZOffset = false;
	bScalePushForceToVelocity = true;
	StandingDownwardForceScale = 1.0f;
	InitialPushForceFactor = 500.0f;
	PushForceFactor = 750000.0f;
	PushForcePointZOffsetFactor = -0.75f;
	TouchForceFactor = 1.0f;
	MinTouchForce = -1.0f;
	MaxTouchForce = 250.0f;
	RepulsionForce = 2.5f;
	//CharacterMovement
	bIgnoreClientMovementErrorChecksAndCorrection = false;
	bServerAcceptClientAuthoritativePosition = false; // TODO this?
	//CharacterMovement- Avoidance todo look at this?
	bUseRVOAvoidance = false;
	AvoidanceConsiderationRadius = 500.0f;
	AvoidanceWeight = 0.0f; //todo this?
	//  todo avoidance groups?
	//RootMotion
	bAllowPhysicsRotationDuringAnimRootMotion = false;
	FormerBaseVelocityDecayHalfLife = 0.0f;
	//CharacterMovement- NavMeshMovement
	NavWalkingFloorDistTolerance = 10.0f;
	//MovementComponent
	bUpdateOnlyIfRendered = false;
	bAutoUpdateTickRegistration = true;
	bTickBeforeOwner = true;
	bAutoRegisterUpdatedComponent = true;
	bAutoRegisterPhysicsVolumeUpdates = true;
	bComponentShouldUpdatePhysicsVolume = true;
	// SetIsReplicated(true);
	bAutoActivate = true;
	//NavMovement- Properties
	NavMovementProperties = FNavMovementProperties(
		0.0f,
		true,
		true,
		false,
		true
	);
	//NavMovement- MovementCapabilities
	NavAgentProps = FNavAgentProperties();
	NavAgentProps.bCanCrouch = false;
	NavAgentProps.bCanJump = true;
	NavAgentProps.bCanWalk = true;
	NavAgentProps.bCanSwim = false;
	NavAgentProps.bCanFly = false;
}
