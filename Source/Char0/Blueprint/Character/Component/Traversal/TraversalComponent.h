#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Animation/LttnAnimInstance.h"
#include "Char0/Blueprint/Data/Traversal/Properties_Traversal.h"
#include "Char0/Blueprint/Data/Traversal/TraversalCheckResult.h"
#include "Components/ActorComponent.h"
#include "TraversalComponent.generated.h"

class UPlayMontageCallbackProxy;
struct FTraversalChooserOutputs;
class UChooserTable;
struct FTraversalChooserInputs;
struct FPoseHistoryReference;
struct FTraversalCheckInputs;
class ALttnCharacter;
struct FTraversalCheckResult;
struct FProperties_Traversal;

UCLASS()
class CHAR0_API UTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	FProperties_Traversal Properties;
	UPROPERTY()
	FTraversalCheckResult CheckResult;

	UPROPERTY()
	ALttnCharacter* LttnCharacter;
	UPROPERTY()
	ULttnAnimInstance* LttnAnimInstance;

	UPROPERTY()
	FVector ActorLocation;
	UPROPERTY()
	float CapsuleRadius;
	UPROPERTY()
	float CapsuleHalfHeight;

	UPROPERTY()
	FVector HasRoomCheckBackLedgeLocation;
	UPROPERTY()
	FVector HasRoomCheckFrontLedgeLocation;

	UPROPERTY()
	UPlayMontageCallbackProxy* PlayMontageCallbackProxy;
	
	// UPROPERTY()
	// FTimerHandle RetriggerHandle;
	// UPROPERTY()
	// bool bRetriggerTimerActive = false;

protected:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery> TraversableTraceChannel;
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery> VisibilityTraceChannel;
	UPROPERTY(EditDefaultsOnly)
	UChooserTable* MontageChooser;

public:

	UPROPERTY()
	bool bDoingAction;
	
	UTraversalComponent();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	bool TryTraversal(const FTraversalCheckInputs& Inputs);

private:
	UFUNCTION()
	void CacheValues();
	UFUNCTION()
	bool FindAndSetLedges(const FTraversalCheckInputs& Input);
	UFUNCTION()
	bool FrontLedgeHasRoom();
	UFUNCTION()
	bool FindBackLedgeAndSetDepth();
	UFUNCTION()
	void FindBackFloorAndSetDepth();
	UFUNCTION()
	void SendFrontLedgeLocationToAnim();

	UFUNCTION()
	void SetWarpTargets() const;
	UFUNCTION()
	void IgnoreComponentWhenMoving(bool bShouldIgnore) const;
	UFUNCTION()
	void SetMovementMode(EMovementMode NewMode);
	UFUNCTION()
	void SetReplicationBehavior(bool ClientAuthoritative);

	UFUNCTION(Server, Reliable)
	void Server_PerformTraversal(const FTraversalCheckResult& Result);
	void Server_PerformTraversal_Implementation(const FTraversalCheckResult& Result);
	
	UFUNCTION()
	void FinishTraversalAction();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PerformTraversal(const FTraversalCheckResult& Result);
	void Multicast_PerformTraversal_Implementation(const FTraversalCheckResult& Result);

	UFUNCTION()
	void ChooseMontage(ULttnAnimInstance* AnimInstance, FTraversalChooserInputs ChooserInputs);
	UFUNCTION()
	FTraversalChooserInputs GetChooserInputs();
	
	UFUNCTION()
	void PlayMontage(USkeletalMeshComponent* Mesh, UAnimMontage* Montage, float PlayRate, float StartTime);
	UFUNCTION()
	void MontageComplete(FName NotifyName);

	ALttnCharacter* GetLttnCharacter();
	ULttnAnimInstance* GetLttnAnimInstance();

	static EMovementMode ToMovementMode(ETraversalActionType ActionType);
};
