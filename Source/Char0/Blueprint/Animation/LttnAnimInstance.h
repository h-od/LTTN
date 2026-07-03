#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNodeReference.h"
#include "Animation/TrajectoryTypes.h"
#include "BoneControllers/AnimNode_FootPlacement.h"
#include "Char0/Blueprint/Data/Movement/Properties_Animation.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "LttnAnimInstance.generated.h"

class UChooserTable;
struct FAnimUpdateContext;
enum class EOrientationWarpingSpace : uint8;
enum class EOffsetRootBoneMode : uint8;
class UPoseSearchDatabase;
struct FFootPlacementPlantSettings;
struct FPoseSearchTrajectoryData;
class ALttnCharacter;
struct FPoseHistoryReference;
struct FProperties_Animation;
enum class EGait : uint8;
enum class EMovementState : uint8;
enum class ERotationMode : uint8;
enum class EMovementType : uint8;

UCLASS()
class CHAR0_API ULttnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY()
	ALttnCharacter* LttnCharacter;
	
	UPROPERTY()
	bool bHasOwningActor;

protected:
	UPROPERTY(EditDefaultsOnly)
	UChooserTable* PoseChooser;
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery> VisibilityTraceChannel;
private:
	// Essential Values
	UPROPERTY()
	FTransform CharTransform;
	UPROPERTY()
	FTransform CharTransform_LastFrame;
	UPROPERTY()
	FTransform RootTransform;
	UPROPERTY()
	FVector Acceleration;
	UPROPERTY()
	FVector Acceleration_LastFrame;
	UPROPERTY()
	float AccelerationAmount;
	UPROPERTY()
	bool bHasVelocity;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FVector Velocity_LastFrame;
	UPROPERTY()
	FVector RelativeAcceleration;
	UPROPERTY()
	FVector VelocityAcceleration;
protected:
	UPROPERTY(BlueprintReadOnly)//Used in BlendStack
	FVector LastNonZeroVelocity;
	UPROPERTY(BlueprintReadOnly)//Used in CHT_PoseSearchDatabase
	float Speed2D;
private:
	UPROPERTY()
	float HeavyLandSpeedThreshold;
protected:
	UPROPERTY(BlueprintReadWrite, Category="Essential") //Set in ABP_LttnCharacter (ThreadSafe?)
	FTransform InteractionTransform;
	//States
	UPROPERTY(BlueprintReadOnly, Category="Essential Values") //Used for Property Access functions
	FProperties_Animation Properties;
public:
	UPROPERTY(BlueprintReadOnly, Category="States")//Used in CHT_PoseSearchDatabase
	EMovementType MovementMode;
private:
	UPROPERTY()
	EMovementType MovementMode_LastFrame;
public:
	UPROPERTY(BlueprintReadOnly, Category="States")//Used in CHT_PoseSearchDatabase
	ERotationMode RotationMode;
private:
	UPROPERTY()
	ERotationMode RotationMode_LastFrame;
public:
	UPROPERTY(BlueprintReadOnly, Category="States")//Used in CHT_PoseSearchDatabase
	EMovementState MovementState;
private:
	UPROPERTY()
	EMovementState MovementState_LastFrame;
public:
	UPROPERTY(BlueprintReadOnly, Category="States")//Used in CHT_PoseSearchDatabase
	EGait Gait;
private:
	UPROPERTY()
	EGait Gait_LastFrame;
	// Trajectory
	UPROPERTY()
	FPoseSearchTrajectoryData TrajectoryGenerationData_Idle;
	UPROPERTY()
	FPoseSearchTrajectoryData TrajectoryGenerationData_Moving;
protected:
	UPROPERTY(BlueprintReadOnly)//Used in AnimGraph
	FTransformTrajectory Trajectory;
	UPROPERTY(BlueprintReadOnly)//Used in CHT_PoseSearchDatabase
	FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;
private:
	UPROPERTY()
	float PreviousDesiredControllerYaw;
	UPROPERTY()
	FVector Trj_FutureVelocity;
	//Footplacement
	UPROPERTY()
	FFootPlacementPlantSettings PlantSettingsDefault;
	UPROPERTY()
	FFootPlacementPlantSettings PlantSettingsStops;
	UPROPERTY()
	FFootPlacementInterpolationSettings InterpolationSettingsDefault;
	UPROPERTY()
	FFootPlacementInterpolationSettings InterpolationSettingsStops;
protected:
	//MotionMatching
	UPROPERTY(BlueprintReadOnly)//Used in BlendStack
	TArray<FName> CurrentPoseDatabaseTags;
	
public:
	explicit ULttnAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// Property Access  
	UFUNCTION(BlueprintNativeEvent, Category="Property Access", meta=(BlueprintThreadSafe))
	FTransform GetProperty_ActorTransform();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	FVector GetProperty_InputAcceleration();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	float GetProperty_CurrentMaxAcceleration();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	float GetProperty_CurrentMaxDeceleration();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	FVector GetProperty_Velocity();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	float GetProperty_DeltaSecs();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	EMovementType GetProperty_MovementType();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	ERotationMode GetProperty_RotationMode();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access", meta=(BlueprintThreadSafe))
	EGait GetProperty_Gait();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	FRotator GetProperty_OrientationIntent();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	bool GetProperty_WantsToAim();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	float GetProperty_LandVelocityZ();
	UFUNCTION(BlueprintNativeEvent, Category="Property Access")
	UAnimInstance* GetProperty_AnimInstance();
	// Property Access - End
public:
	UFUNCTION(BlueprintCallable)
	FProperties_Animation GetProperties_Animation();
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FTransform GetInteractionTransform();
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	EGait GetGait();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetInteraction(FTransform Transform);

	// Motion Matching
	UFUNCTION()
	FPoseHistoryReference GetPoseHistoryReference();

protected:
	// AnimNodeRef
	UFUNCTION(BlueprintNativeEvent, Category="AnimNode References")
	FAnimNodeReference GetOffSetRoot();
	UFUNCTION(BlueprintNativeEvent, Category="AnimNode References")
	FAnimNodeReference GetPoseHistory();

private:
	//EventGraph
	UFUNCTION()
	void UpdateProperties_Animation();
	UFUNCTION()
	void UpdateTrajectory();
	UFUNCTION()
	void UpdateEssentialValues();
	UFUNCTION()
	void UpdateStates();
	UFUNCTION()
	void UpdateLogic();
	
protected:
	//Movement Analysis
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph //called from bp
	bool IsMoving() const;
public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool IsStarting() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool IsPivoting() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool ShouldTurnInPlace();
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool ShouldSpinTransition() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool JustTraversed() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool JustLanded_Light() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in CHT_PoseSearchDatabase
	bool JustLanded_Heavy() const;
private:
	UFUNCTION()
	float GetTrajectoryTurnAngle() const;
protected:
	//RootOffset
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	EOffsetRootBoneMode GetOffsetRootRotationMode() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	EOffsetRootBoneMode GetOffsetRootTranslationMode() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	float GetOffsetRootTranslationHalfLife() const;
	//AimOffset
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	bool EnableAimOffset() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	FVector2D GetAimOffset() const;
	//Additive Lean
	UFUNCTION()
	FVector CalculateRelativeAcceleration();
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	FVector2D GetLeanAmount();
	//Steering
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in BlendStack
	bool EnableSteering(FAnimNodeReference Node) const; //TODO not working as expected: BlueprintPure doesn't seem to work -> recreated in bp 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in BlendStack
	FQuat GetDesiredFacing() const;//TODO not working as expected: BlueprintPure doesn't seem to work -> recreated in bp
	//Foot Placement
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	FFootPlacementPlantSettings GetFootPlacementPlantSettings() const;
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	FFootPlacementInterpolationSettings GetFootPlacementInterpolationSettings() const;
	//Motion Matching
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))//Used in AnimGraph
	void UpdateMotionMatching(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))//Used in AnimGraph
	void UpdateMotionMatchingPost(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	float GetMMBlendTime() const;

private:
	UFUNCTION()
	EPoseSearchInterruptMode GetMMInterruptMode() const;

protected:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))//Used in AnimGraph
	float GetMMNotifyRecencyTimeOut() const;
};
