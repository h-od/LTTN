#include "LttnCharacter.h"

#include <string>

#include "EnhancedInputComponent.h"
#include "KismetAnimationLibrary.h"
#include "Bot/BotCharacter.h"
#include "Char0/Blueprint/Controller/LttnController.h"
#include "Char0/Blueprint/Data/Movement/Gait.h"
#include "Char0/Blueprint/Data/Movement/MovementType.h"
#include "Char0/Blueprint/Data/Movement/RotationMode.h"
#include "Char0/Blueprint/Data/Traversal/TraversalCheckInputs.h"
#include "Char0/Blueprint/Data/Camera/CameraMode.h"
#include "Char0/Blueprint/Data/Camera/CameraStyle.h"
#include "Char0/Blueprint/Data/Camera/PropertiesForCamera.h"
#include "Char0/Blueprint/Data/Interaction/InteractableType.h"
#include "Char0/Blueprint/Data/Movement/PlayerInputState.h"
#include "Char0/Blueprint/Data/Movement/Properties_Animation.h"
#include "Char0/Blueprint/Data/Sound/SoundParams.h"
#include "Char0/Blueprint/Data/Sound/SoundSide.h"
#include "Component/Gameplay/GameplayComponent.h"
#include "Component/Movement/LttnMovementComponent.h"
#include "Component/Sound/SoundEventsComponent.h"
#include "Component/Traversal/TraversalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ALttnCharacter::ALttnCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<ULttnMovementComponent>(CharacterMovementComponentName)
)
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	bUseControllerRotationYaw = false;
	InputState = FPlayerInputState(false, false);
	CameraStyle = ECameraStyle::Medium;
	Gait = EGait::Run;
	RunSpeeds = FVector(500, 350, 300);
	SprintSpeeds = FVector(700, 700, 700);
	CrouchSpeeds = FVector(225, 200, 180);
	bIsRagdolling = false;


	Camera = CreateDefaultSubobject<UGameplayCameraComponent>("Camera");
	Camera->SetupAttachment(Cast<USceneComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass())), FName("Mesh"));

	GameplayComponent = CreateDefaultSubobject<UGameplayComponent>(TEXT("GameplayComponent"));
	PreTickComponent = CreateDefaultSubobject<UPreTickComponent>(TEXT("PreTickComponent"));
	TraversalComponent = CreateDefaultSubobject<UTraversalComponent>(TEXT("TraversalComponent"));
	SoundComponent = CreateDefaultSubobject<USoundEventsComponent>(TEXT("SoundComponent"));
	// MovementComponent = Cast<ULttnMovementComponent>(GetCharacterMovement());//CreateDefaultSubobject<ULttnMovementComponent>("MovementComponent");
}

void ALttnCharacter::BeginPlay()
{
	Super::BeginPlay();
	PreTickComponent->OnTick.AddDynamic(this, &ALttnCharacter::PreMovementTick);

	GetCharacterMovement()->AddTickPrerequisiteComponent(PreTickComponent);

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		OnCharacterMovementUpdated.AddDynamic(this, &ALttnCharacter::OnMovementUpdated);
	}
}

void ALttnCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	PreTickComponent->OnTick.RemoveDynamic(this, &ALttnCharacter::PreMovementTick);

	GetCharacterMovement()->RemoveTickPrerequisiteComponent(PreTickComponent);
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		OnCharacterMovementUpdated.RemoveDynamic(this, &ALttnCharacter::OnMovementUpdated);
	}
}

void ALttnCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	LttnController = Cast<ALttnController>(NewController);
	Client_Possessed();
}

void ALttnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALttnCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ALttnCharacter::StartedMoving);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ALttnCharacter::StoppedMoving);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ALttnCharacter::StoppedMoving);
		EnhancedInputComponent->BindAction(Look_MouseAction, ETriggerEvent::Triggered, this, &ALttnCharacter::Look_MouseTriggered);
		EnhancedInputComponent->BindAction(Look_GamepadAction, ETriggerEvent::Triggered, this, &ALttnCharacter::Look_GamepadTriggered);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ALttnCharacter::SprintTriggered);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ALttnCharacter::SprintStarted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ALttnCharacter::SprintCompleted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ALttnCharacter::SprintCompleted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ALttnCharacter::JumpTriggered);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALttnCharacter::JumpStarted);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ALttnCharacter::AimStarted);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ALttnCharacter::AimFinished);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Canceled, this, &ALttnCharacter::AimFinished);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ALttnCharacter::Interact);
		// EnhancedInputComponent->BindAction(ToggleViewAction, ETriggerEvent::Started, this, &ALttnCharacter::ToggleViewStarted);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ALttnCharacter::Pause);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ALttnCharacter::FireStarted);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &ALttnCharacter::FireStopped);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ALttnCharacter::FireStopped);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ALttnCharacter::ReloadStarted);

		EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Started, this, &ALttnCharacter::ZoomInPressed);
		EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Started, this, &ALttnCharacter::ZoomOutPressed);
		EnhancedInputComponent->BindAction(Zoom_MouseAction, ETriggerEvent::Started, this, &ALttnCharacter::ZoomPressed);
	}
}

void ALttnCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALttnCharacter, InputState);
	DOREPLIFETIME(ALttnCharacter, bIsDead);
}

void ALttnCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	const float Volume = UKismetMathLibrary::MapRangeClamped(
		UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity),
		0.0f, 500.0f,
		0.5f, 1.0f
	);
	//TODO why is this not a notify from the Animation?
	SoundComponent->PlaySound(JumpEventGameplayTag, FSoundParams(ESoundSide::None, Volume, 1.0f));
}

void ALttnCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	LandVelocity = GetCharacterMovement()->Velocity;

	const float Volume = UKismetMathLibrary::MapRangeClamped(
		LandVelocity.Z,
		-500.0f, -900.0f,
		0.5f, 1.5f
	);
	//TODO why is this not a notify from the Animation?
	SoundComponent->PlaySound(LandEventGameplayTag, FSoundParams(ESoundSide::None, Volume, 1.0f));
	bJustLanded = true;
	GetWorld()->GetTimerManager().ClearTimer(JustLandedTimerHandle);

	GetWorldTimerManager().SetTimer(JustLandedTimerHandle, this, &ALttnCharacter::DidLand, 0.3f, false);
}

float ALttnCharacter::TakeDamage(const float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser->IsA(ABotCharacter::StaticClass()))
	{
		GameplayComponent->TakeDamage(DamageCauser->GetActorForwardVector());
	}
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ALttnCharacter::SetNewPlayerInfo()
{
	Client_SetNewPlayerInfo();
}

FProperties_Traversal ALttnCharacter::GetProperties_Traversal() const
{
	EMovementType MovementMode;
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->MovementMode == MOVE_None or MovementComponent->MovementMode == MOVE_Walking or MovementComponent->MovementMode == MOVE_NavWalking)
	{
		MovementMode = EMovementType::OnGround;
	}
	else
	{
		MovementMode = EMovementType::InAir;
	}
	return FProperties_Traversal(
		GetCapsuleComponent(),
		GetMesh(),
		FindComponentByClass<UMotionWarpingComponent>(),
		MovementMode,
		Gait,
		MovementComponent->Velocity.Size2D()
	);
}

FProperties_Animation ALttnCharacter::GetProperties_Animation() const
{
	EMovementType MovementMode;
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->MovementMode == MOVE_Falling or MovementComponent->MovementMode == MOVE_Swimming)
	{
		MovementMode = EMovementType::InAir;
	}
	else
	{
		MovementMode = EMovementType::OnGround;
	}

	return FProperties_Animation(
		InputState,
		MovementMode,
		MovementComponent->bOrientRotationToMovement ? ERotationMode::OrientToMovement : ERotationMode::Aim,
		Gait,
		GetActorTransform(),
		MovementComponent->Velocity,
		MovementComponent->GetCurrentAcceleration(),
		MovementComponent->GetMaxAcceleration(),
		MovementComponent->BrakingDecelerationWalking,
		GetActorRotation(),
		IsLocallyControlled() ? GetControlRotation() : GetBaseAimRotation(),
		bJustLanded,
		LandVelocity
	);
}

FPropertiesForCamera ALttnCharacter::GetPropertiesCamera() const
{
	return FPropertiesForCamera(
		CameraStyle,
		InputState.bWantsToAim ? ECameraMode::Aim : ECameraMode::Free
	);
}

FPlayerManager ALttnCharacter::GetPlayerManager() const
{
	return GameplayComponent->GetPlayerManager();
}

void ALttnCharacter::StartedMoving()
{
	GameplayComponent->SetIsMoving(true);
}

void ALttnCharacter::StoppedMoving()
{
	GameplayComponent->SetIsMoving(false);
}

void ALttnCharacter::PlayerDead()
{
	//TODO is anyone alive? Spectate else summary
}

void ALttnCharacter::UpdateWeaponProjectiles(const int32 Count) const
{
	LttnController->UpdateWeaponProjectiles(Count);
}

FVector ALttnCharacter::GetWeaponLocation_Implementation()
{
	return FVector::ZeroVector;
}

void ALttnCharacter::PlayerIsReloading(const float ReloadTime) const
{
	LttnController->PlayerIsReloading(ReloadTime);
}

void ALttnCharacter::PlayerReloaded(const int32 WeaponProjectileCount, const int32 PlayerProjectileCount) const
{
	LttnController->PlayerReloaded(WeaponProjectileCount, PlayerProjectileCount);
}

void ALttnCharacter::SetMaxHealth()
{
	Client_SetMaxHealth();
}

void ALttnCharacter::StaminaDepleted()
{
	InputState.bWantsToSprint = false;
	Server_InputStateUpdated(InputState);
}

void ALttnCharacter::SetPlayerStamina(const float Stamina) const
{
	LttnController->SetPlayerStamina(Stamina);
}

void ALttnCharacter::ShowScore(const int32 CurrentScore, const int32 Cost) const
{
	LttnController->ShowScore(CurrentScore, Cost);
}

void ALttnCharacter::ShowMaxPlayerProjectiles(const int32 MaxProjectiles) const
{
	LttnController->ShowMaxPlayerProjectiles(MaxProjectiles);
}

void ALttnCharacter::ShowMaxWeaponProjectiles(const int32 MaxProjectiles) const
{
	LttnController->ShowMaxWeaponProjectiles(MaxProjectiles);
}

void ALttnCharacter::ShowResupplyCooldown(const float TimerRemaining) const
{
	LttnController->ShowResupplyCooldown(TimerRemaining);
}

void ALttnCharacter::ShowPlayerProjectiles(const int32 Projectiles) const
{
	LttnController->ShowPlayerProjectiles(Projectiles);
}

void ALttnCharacter::CanInteract(const EInteractableType Type)
{
	if (!IsLocallyControlled() or Type == EInteractableType::NoInteraction)
	{
		return;
	}

	ALttnController* C = GetLttnController();

	switch (Type)
	{
	case EInteractableType::StartGame:
		Interactable = Type;
		C->ShowCanInteract(C->CanStartGame());
		break;
	case EInteractableType::StartLevel:
		Interactable = Type;
		C->ShowCanInteract(C->CanStartLevel());
		break;
	case EInteractableType::Resupply:
		Interactable = Type;
		C->ShowCanInteract(GameplayComponent->CanResupply());
		break;
	case EInteractableType::UpgradeWeapon:
		Interactable = Type;
		C->ShowCanInteract(GameplayComponent->CanUpgradeWeapon());
		break;
	case EInteractableType::UpgradeHealth:
		Interactable = Type;
		C->ShowCanInteract(GameplayComponent->CanUpgradeHealth());
		break;
	case EInteractableType::UpgradeStamina:
		Interactable = Type;
		C->ShowCanInteract(GameplayComponent->CanUpgradeStamina());
		break;
	case EInteractableType::UpgradeReloadSpeed:
		Interactable = Type;
		C->ShowCanInteract(GameplayComponent->CanUpgradeReloadSpeed());
		break;
	case EInteractableType::UpgradeProjectileCapacity:
		Interactable = Type;
		C->ShowCanInteract(GameplayComponent->CanUpgradeProjectileCapacity());
		break;
	case EInteractableType::NoInteraction:
		Interactable = Type;
		C->ShowCanInteract(false);
		break;
	}
}

void ALttnCharacter::CantInteract()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	Interactable = EInteractableType::NoInteraction;
	GetLttnController()->ShowCanInteract(false);
}

void ALttnCharacter::Client_Possessed_Implementation()
{
	LttnController = Cast<ALttnController>(GetController());
	Camera->ActivateCameraForPlayerController(Cast<APlayerController>(GetController()), true, EGameplayCameraComponentActivationMode::Push);
}

void ALttnCharacter::Client_SetNewPlayerInfo_Implementation()
{
	GameplayComponent->SetNewPlayerInfo();
}

void ALttnCharacter::MoveTriggered(const FInputActionValue& InputActionValue)
{
	const float Yaw = GetControlRotation().Yaw;
	const FRotator YawRotation(0, Yaw, 0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawRotation);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(YawRotation);
	const FVector2D Input = InputActionValue.Get<FVector2D>();
	const FVector2D ScaleValue = UKismetMathLibrary::Normal2D(Input);

	// UKismetSystemLibrary::PrintString(GetWorld(), "ALttnCharacter::Look_GamepadTriggered X: " + FString::SanitizeFloat(ScaleValue.X) + ", Y: " + FString::SanitizeFloat(ScaleValue.X), true, true, FLinearColor::Red, 5.0f);
	
	AddMovementInput(RightVector, ScaleValue.X, false);
	AddMovementInput(ForwardVector, ScaleValue.Y, false);
}

void ALttnCharacter::Look_MouseTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxis = InputActionValue.Get<FVector2D>();
	float Yaw;
	float Pitch;

	if (InputState.bWantsToAim)
	{
		Yaw = LookAxis.X * AimMouseSens / 100;
		Pitch = LookAxis.Y * AimMouseSens / 100;
	}
	else
	{
		Yaw = LookAxis.X * MouseSens / 100;
		Pitch = LookAxis.Y * MouseSens / 100;
	}

	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
		// if (this->GetName().EndsWith("_0"))
		// {
		// 	UKismetSystemLibrary::PrintString(GetWorld(), "ALttnCharacter::Look_MouseTriggered " + FString::SanitizeFloat(Pitch) + "," + FString::SanitizeFloat(Yaw), true, true, FLinearColor::Red,
		// 	                                  5.0f);
		// }
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("ALttnCharacter::Look_MouseTriggered controller null"));
	}
}

void ALttnCharacter::Look_GamepadTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxis = InputActionValue.Get<FVector2D>();
	const float WorldDeltaSecs = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	float Yaw;
	float Pitch;

	if (InputState.bWantsToAim)
	{
		Yaw = LookAxis.X * (AimStickSens / 100) * WorldDeltaSecs;
		Pitch = LookAxis.Y * (AimStickSens / 100) * WorldDeltaSecs;
	}
	else
	{
		Yaw = LookAxis.X * (StickSens / 100) * WorldDeltaSecs;
		Pitch = LookAxis.Y * (StickSens / 100) * WorldDeltaSecs;
	}

	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("ALttnCharacter::Look_GamepadTriggered controller null"));
	}
}

void ALttnCharacter::SprintTriggered(const FInputActionValue& InputActionValue)
{
	InputState.bWantsToSprint = InputActionValue.Get<bool>();
	Server_InputStateUpdated(InputState);
}

void ALttnCharacter::SprintStarted()
{
	GameplayComponent->StartSprinting();
}

void ALttnCharacter::SprintCompleted()
{
	GameplayComponent->StopSprinting();
}

void ALttnCharacter::JumpTriggered()
{
	if (!TraversalComponent->bDoingAction)
	{
		TraversalComponent->TryTraversal(GetTraversalInputs());
	}
}

void ALttnCharacter::JumpStarted()
{
	if (!TraversalComponent->bDoingAction)
	{
		if (TraversalComponent->TryTraversal(GetTraversalInputs()))
		{
			Jump();
		}
	}
}

void ALttnCharacter::AimStarted()
{
	InputState.bWantsToAim = true;
	Server_InputStateUpdated(InputState);
}

void ALttnCharacter::AimFinished()
{
	InputState.bWantsToAim = false;
	Server_InputStateUpdated(InputState);
}

void ALttnCharacter::Interact()
{
	//TODO Res
	//TODO if can interact interact otherwise reload (on controller)

	switch (Interactable)
	{
	case EInteractableType::StartGame:
		GetLttnController()->StartGame();
		CanInteract(Interactable);
		break;
	case EInteractableType::StartLevel:
		GetLttnController()->StartLevel();
		CanInteract(Interactable);
		break;
	case EInteractableType::Resupply:
		GameplayComponent->Resupply();
		CanInteract(Interactable);
		break;
	case EInteractableType::UpgradeWeapon:
		if (GameplayComponent->CanUpgradeWeapon())
		{
			GameplayComponent->UpgradeWeapon();
		}
		CanInteract(Interactable);
		break;
	case EInteractableType::UpgradeHealth:
		if (GameplayComponent->CanUpgradeHealth())
		{
			GameplayComponent->UpgradeHealth();
		}
		CanInteract(Interactable);
		break;
	case EInteractableType::UpgradeStamina:
		if (GameplayComponent->CanUpgradeStamina())
		{
			GameplayComponent->UpgradeStamina();
		}
		CanInteract(Interactable);
		break;
	case EInteractableType::UpgradeReloadSpeed:
		if (GameplayComponent->CanUpgradeReloadSpeed())
		{
			GameplayComponent->UpgradeReloadSpeed();
		}
		CanInteract(Interactable);
		break;
	case EInteractableType::UpgradeProjectileCapacity:
		if (GameplayComponent->CanUpgradeProjectileCapacity())
		{
			GameplayComponent->UpgradeProjectileCapacity();
		}
		CanInteract(Interactable);
		break;
	case EInteractableType::NoInteraction:
		break;
	}
}

void ALttnCharacter::Pause()
{
}

void ALttnCharacter::FireStarted()
{
	GameplayComponent->StartFiring();
}

void ALttnCharacter::FireStopped()
{
	GameplayComponent->StopFiring();
}

void ALttnCharacter::ReloadStarted()
{
	GameplayComponent->Reload();
}

void ALttnCharacter::ZoomInPressed()
{
	switch (CameraStyle)
	{
	case ECameraStyle::Close:
	case ECameraStyle::Medium:
		CameraStyle = ECameraStyle::Close;
		break;
	case ECameraStyle::Far:
		CameraStyle = ECameraStyle::Medium;
		break;
	case ECameraStyle::Debug:
		CameraStyle = ECameraStyle::Far;
		break;
	}
	UGameplayStatics::PlaySound2D(GetWorld(), CameraChangeSound);
}

void ALttnCharacter::ZoomOutPressed()
{
	switch (CameraStyle)
	{
	case ECameraStyle::Close:
		CameraStyle = ECameraStyle::Medium;
		break;
	case ECameraStyle::Medium:
		CameraStyle = ECameraStyle::Far;
		break;
	case ECameraStyle::Far:
	case ECameraStyle::Debug:
		CameraStyle = ECameraStyle::Debug;
		break;
	}
	UGameplayStatics::PlaySound2D(GetWorld(), CameraChangeSound);
}

void ALttnCharacter::ZoomPressed(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<float>() > 0)
	{
		ZoomInPressed();
	}
	else
	{
		ZoomOutPressed();
	}
}

void ALttnCharacter::PreMovementTick()
{
	UpdateRotation_PreTick();
	UpdateMovement_PreTick();
}

void ALttnCharacter::UpdateRotation_PreTick() const
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bUseControllerDesiredRotation = InputState.bWantsToAim;
	MovementComponent->bOrientRotationToMovement = !InputState.bWantsToAim;

	// MovementComponent->RotationRate = FRotator(0.0f, /*MovementComponent->IsFalling() ? 200.0f :*/ -1.0f, 0.0f);
}

void ALttnCharacter::UpdateMovement_PreTick()
{
	Gait = GetDesiredGait();
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->MaxAcceleration = CalculateMaxAcceleration();
	MovementComponent->BrakingDecelerationWalking = CalculateBreakingDeceleration();
	MovementComponent->GroundFriction = CalculateGroundFriction();
	MovementComponent->MaxWalkSpeed = CalculateMaxSpeed();
	MovementComponent->MaxWalkSpeedCrouched = CalculateMaxCrouchSpeed();
}

EGait ALttnCharacter::GetDesiredGait() const
{
	if (CanSprint())
	{
		return EGait::Sprint;
	}
	return EGait::Run;
}

float ALttnCharacter::CalculateMaxAcceleration() const
{
	if (Gait == EGait::Sprint)
	{
		return UKismetMathLibrary::MapRangeClamped(
			GetCharacterMovement()->Velocity.Size2D(),
			300.0f,
			700.0f,
			800.0f,
			300.0f
		);
	}
	return 800.0f;
}

float ALttnCharacter::CalculateBreakingDeceleration() const
{
	if (HasMovementInputVector())
	{
		return 500.0f;
	}
	return 2000.0f;
}

float ALttnCharacter::CalculateGroundFriction() const
{
	if (Gait == EGait::Sprint)
	{
		return UKismetMathLibrary::MapRangeClamped(
			GetCharacterMovement()->Velocity.Size2D(),
			0.0f,
			500.0f,
			5.0f,
			3.0f
		);
	}
	return 5.0f;
}

float ALttnCharacter::CalculateMaxSpeed() const
{
	float StrafeSpeedMap;
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement(); MovementComponent->bUseControllerDesiredRotation)
	{
		const float InTime = UKismetMathLibrary::Abs(UKismetAnimationLibrary::CalculateDirection(MovementComponent->Velocity, GetActorRotation()));
		StrafeSpeedMap = StrafeSpeedMapCurve->GetFloatValue(InTime);
	}
	else
	{
		StrafeSpeedMap = 0.0f;
	}

	FVector Speeds;
	if (Gait == EGait::Sprint)
	{
		Speeds = SprintSpeeds;
	}
	else
	{
		Speeds = RunSpeeds;
	}

	if (StrafeSpeedMap < 1.0f)
	{
		return UKismetMathLibrary::MapRangeClamped(
			StrafeSpeedMap,
			0.0,
			1.0,
			Speeds.X,
			Speeds.Y
		);
	}
	return UKismetMathLibrary::MapRangeClamped(
		StrafeSpeedMap,
		1.0,
		2.0,
		Speeds.Y,
		Speeds.Z
	);
}

float ALttnCharacter::CalculateMaxCrouchSpeed() const
{
	float StrafeSpeedMap;
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement(); MovementComponent->bOrientRotationToMovement)
	{
		StrafeSpeedMap = 0.0f;
	}
	else
	{
		const float InTime = UKismetMathLibrary::Abs(UKismetAnimationLibrary::CalculateDirection(MovementComponent->Velocity, GetActorRotation()));
		StrafeSpeedMap = StrafeSpeedMapCurve->GetFloatValue(InTime);
	}

	if (StrafeSpeedMap < 1.0f)
	{
		return UKismetMathLibrary::MapRangeClamped(
			StrafeSpeedMap,
			0.0,
			1.0,
			CrouchSpeeds.X,
			CrouchSpeeds.Y
		);
	}
	return UKismetMathLibrary::MapRangeClamped(
		StrafeSpeedMap,
		1.0,
		2.0,
		CrouchSpeeds.Y,
		CrouchSpeeds.Z
	);
}

bool ALttnCharacter::HasMovementInputVector() const
{
	return !GetPendingMovementInputVector().Equals(FVector::ZeroVector, 0.0f);
}

bool ALttnCharacter::CanSprint() const
{
	if (!InputState.bWantsToSprint)
	{
		return false;
	}

	bool bCanSprint;
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement(); MovementComponent->bOrientRotationToMovement)
	{
		bCanSprint = true;
	}
	else
	{
		FVector Acceleration;
		if (IsLocallyControlled())
		{
			Acceleration = GetPendingMovementInputVector();
		}
		else
		{
			Acceleration = MovementComponent->GetCurrentAcceleration();
		}

		bCanSprint = UKismetMathLibrary::Abs(
			UKismetMathLibrary::NormalizedDeltaRotator(
				GetActorRotation(), Acceleration.ToOrientationRotator()
			).Yaw
		) < 50.0f;
	}

	return bCanSprint and InputState.bWantsToSprint;
}

void ALttnCharacter::OnMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	const bool bIsMovingOnGround = GetCharacterMovement()->IsMovingOnGround();
	if (bIsMovingOnGround != bWasMovingOnGroundLastFrameSimulated)
	{
		if (bIsMovingOnGround)
		{
			LandVelocity = OldVelocity;
			const float Volume = UKismetMathLibrary::MapRangeClamped(
				LandVelocity.Z,
				-500.0f, -900.0f,
				0.5f, 1.5f
			);
			SoundComponent->PlaySound(LandEventGameplayTag, FSoundParams(ESoundSide::None, Volume, 1.0f));
		}
		else
		{
			const float Volume = UKismetMathLibrary::MapRangeClamped(
				UKismetMathLibrary::VSizeXY(OldVelocity),
				0.0f, 500.0f,
				0.5f, 1.0f
			);
			SoundComponent->PlaySound(JumpEventGameplayTag, FSoundParams(ESoundSide::None, Volume, 1.0f));
		}
	}
	bWasMovingOnGroundLastFrameSimulated = bIsMovingOnGround;
}

void ALttnCharacter::Server_InputStateUpdated_Implementation(const FPlayerInputState NewInputState)
{
	InputState = NewInputState;
}

FTraversalCheckInputs ALttnCharacter::GetTraversalInputs() const
{
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent->MovementMode == MOVE_Falling or MovementComponent->MovementMode == MOVE_Flying)
	{
		return FTraversalCheckInputs(
			GetActorForwardVector(),
			75.0f,
			FVector::ZeroVector,
			FVector(0.0f, 0.0f, 50.0f),
			30.0f,
			86.0f
		);
	}

	return FTraversalCheckInputs(
		GetActorForwardVector(),
		UKismetMathLibrary::MapRangeClamped(
			GetActorRotation().UnrotateVector(MovementComponent->Velocity).X,
			0.0f,
			500.0f,
			75.0f,
			350.0f
		),
		FVector::ZeroVector,
		FVector::ZeroVector,
		30.0f,
		60.0f
	);
}

void ALttnCharacter::DidLand()
{
	bJustLanded = false;
	GetWorld()->GetTimerManager().ClearTimer(JustLandedTimerHandle);
}

void ALttnCharacter::Client_SetMaxHealth_Implementation()
{
	GameplayComponent->SetMaxHealth();
	GetLttnController()->SetPlayerHealth(1.0f);
}

ALttnController* ALttnCharacter::GetLttnController()
{
	if (!LttnController)
	{
		LttnController = Cast<ALttnController>(GetController());
	}
	return LttnController;
}
