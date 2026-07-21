#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Char0/Blueprint/Actor/Interactable/Interactable.h"
#include "Char0/Blueprint/Data/Interaction/InteractableType.h"
#include "Char0/Blueprint/Data/Movement/PlayerInputState.h"
#include "Char0/Blueprint/Data/Player/PlayerManager.h"
#include "Char0/Blueprint/Data/Traversal/Properties_Traversal.h"
#include "Component/PreTick/PreTickComponent.h"
#include "GameFramework/Character.h"
#include "LttnCharacter.generated.h"

class ALttnController;
class UGameplayComponent;
class ULttnMovementComponent;
class UMovementComponent;
struct FProperties_Animation;
class USoundEventsComponent;
struct FPropertiesForCamera;
class UCameraAsset;
class UGameplayCameraComponent;
class UCameraComponent;
class USpringArmComponent;
class UTraversalComponent;
struct FTraversalChooserInputs;
class ULttnAnimInstance;
struct FTraversalChooserOutputs;
class UInputAction;
struct FTraversalCheckInputs;
enum class EGait : uint8;
enum class ECameraStyle : uint8;
enum class EStickBehaviour : uint8;

UCLASS()
class CHAR0_API ALttnCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	ALttnController* LttnController;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FPlayerInputState InputState;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsDead;

private:
	//Simulated
	UPROPERTY()
	bool bWasMovingOnGroundLastFrameSimulated = true; //TODO true?
	UPROPERTY()
	FVector LastUpdateVelocity;
	//Simulated End

	UPROPERTY(Replicated)
	int32 Id;

	UPROPERTY(Replicated)
	float AimStickSens = 50;
	UPROPERTY(Replicated)
	float AimMouseSens = 50;
	UPROPERTY(Replicated)
	float StickSens = 50;
	UPROPERTY(Replicated)
	float MouseSens = 50;

	UPROPERTY(EditDefaultsOnly)
	UGameplayComponent* GameplayComponent;
	UPROPERTY()
	UPreTickComponent* PreTickComponent;
	UPROPERTY(EditDefaultsOnly)
	UTraversalComponent* TraversalComponent;
	UPROPERTY(EditDefaultsOnly)
	USoundEventsComponent* SoundComponent;
	UPROPERTY(EditDefaultsOnly)
	UGameplayCameraComponent* Camera;

	UPROPERTY()
	ECameraStyle CameraStyle;

	UPROPERTY()
	FTimerHandle JustLandedTimerHandle;

	UPROPERTY(Replicated)
	EGait Gait;
	UPROPERTY()
	FVector RunSpeeds;
	UPROPERTY()
	FVector SprintSpeeds;
	UPROPERTY()
	FVector CrouchSpeeds;
	UPROPERTY()
	bool bJustLanded;
	UPROPERTY()
	FVector LandVelocity;
	UPROPERTY()
	bool bIsRagdolling;
	
	UPROPERTY()
	EInteractableType Interactable = EInteractableType::NoInteraction;
	UPROPERTY()
	int32 PlayerToRevive;

protected:
	// Config
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	UCurveFloat* StrafeSpeedMapCurve;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	USoundBase* CameraChangeSound;
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	FGameplayTag LandEventGameplayTag;
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	FGameplayTag JumpEventGameplayTag;

	// InputActions
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Look_MouseAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Look_GamepadAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* InteractAction;
	// UPROPERTY(EditDefaultsOnly, Category="Input")
	// UInputAction* ToggleViewAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* PauseAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ZoomInAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ZoomOutAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Zoom_MouseAction;
	// InputActions - End

public:
	explicit ALttnCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnJumped_Implementation() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(Client, Reliable)
	void Client_TakeDamage(float Damage, AActor* DamageCauser);
	void Client_TakeDamage_Implementation(float Damage, AActor* DamageCauser);

	void SetNewPlayerInfo();

	UFUNCTION()
	FProperties_Traversal GetProperties_Traversal() const;
	UFUNCTION()
	FProperties_Animation GetProperties_Animation() const;
	UFUNCTION(BlueprintPure)
	FPropertiesForCamera GetPropertiesCamera() const;

	//GameplayComponent
	int32 GetId() const;
	FPlayerManager GetPlayerManager() const;
	void SetPlayerInfo(const FPlayerManager& PlayerManager);
	void StartedMoving();
	void StoppedMoving();
	void PlayerDead();
	void SetRagDoll();
	UFUNCTION(Server, Reliable)
	void Server_SetRagDoll();
	void Server_SetRagDoll_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_SetRagDoll();
	void MC_SetRagDoll_Implementation();
	void UpdateWeaponProjectiles(int32 Count) const;
	UFUNCTION(BlueprintNativeEvent)
	FVector GetWeaponLocation();
	void PlayerIsReloading(float ReloadTime) const;
	void PlayerReloaded(int32 WeaponProjectileCount, int32 PlayerProjectileCount) const;
	void SetMaxHealth();
	void StaminaDepleted();
	void SetPlayerStamina(float NewValue) const;
	void SetPlayerHealth(float NewValue) const;
	void ShowScore(int32 CurrentScore, int32 Cost) const;
	void ShowMaxPlayerProjectiles(int32 MaxProjectiles) const;
	void ShowMaxWeaponProjectiles(int32 MaxProjectiles) const;
	void ShowResupplyCooldown(float TimerRemaining) const;
	void ShowPlayerProjectiles(int32 Projectiles) const;
	
	//InteractableActor
	void CanInteract(EInteractableType Type);
	void CantInteract();
	
	void CanRevive(int32 RevivableId);
	UFUNCTION(Client, Reliable)
	void Client_CanRevive(int32 RevivableId);
	void Client_CanRevive_Implementation(int32 RevivableId);
	
	void CantRevive();
	UFUNCTION(Client, Reliable)
	void Client_CantRevive();
	void Client_CantRevive_Implementation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void EnableCollisionSphere(bool bEnabled);
	
protected:
	UFUNCTION(BlueprintCallable)
	void OverlapStart(AActor* Overlapping);
	
	UFUNCTION(BlueprintCallable)
	void OverlapEnd(AActor* Overlapping);
	
private:
	UFUNCTION(Client, Reliable)
	void Client_Possessed();
	void Client_Possessed_Implementation();
	
	UFUNCTION(Client, Reliable)
	void Client_SetNewPlayerInfo();
	void Client_SetNewPlayerInfo_Implementation();

	//Handle Inputs
	void MoveTriggered(const FInputActionValue& InputActionValue);
	void Look_MouseTriggered(const FInputActionValue& InputActionValue);
	void Look_GamepadTriggered(const FInputActionValue& InputActionValue);
	void SprintTriggered(const FInputActionValue& InputActionValue);
	void SprintStarted();
	void SprintCompleted();
	void JumpTriggered();
	void JumpStarted();
	void AimStarted();
	void AimFinished();
	void Interact();
	void Pause();
	void FireStarted();
	void FireStopped();
	void ReloadStarted();
	void ZoomInPressed();
	void ZoomOutPressed();
	void ZoomPressed(const FInputActionValue& InputActionValue);
	//Handle Inputs End

	UFUNCTION()
	void PreMovementTick();
	UFUNCTION()
	void UpdateRotation_PreTick() const;
	UFUNCTION()
	void UpdateMovement_PreTick();

	UFUNCTION()
	EGait GetDesiredGait() const;
	UFUNCTION()
	float CalculateMaxAcceleration() const;
	UFUNCTION()
	float CalculateBreakingDeceleration() const;
	UFUNCTION()
	float CalculateGroundFriction() const;
	UFUNCTION()
	float CalculateMaxSpeed() const;
	UFUNCTION()
	float CalculateMaxCrouchSpeed() const;
	UFUNCTION()
	bool HasMovementInputVector() const;
	UFUNCTION()
	bool CanSprint() const;

	UFUNCTION()
	void OnMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);

	UFUNCTION(Server, Reliable)
	void Server_InputStateUpdated(FPlayerInputState NewInputState);
	void Server_InputStateUpdated_Implementation(FPlayerInputState NewInputState);

	UFUNCTION()
	FTraversalCheckInputs GetTraversalInputs() const;

	UFUNCTION()
	void DidLand();
	
	UFUNCTION(Client, Reliable)
	void Client_SetMaxHealth();
	void Client_SetMaxHealth_Implementation();
	
	UFUNCTION()
	ALttnController* GetLttnController();
};
