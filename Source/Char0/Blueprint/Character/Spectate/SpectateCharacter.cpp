#include "SpectateCharacter.h"

#include "EnhancedInputComponent.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASpectateCharacter::BeginPlay()
{
	Super::BeginPlay();
	//TODO widget
}

void ASpectateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(GamepadLookAction, ETriggerEvent::Triggered, this, &ASpectateCharacter::GamepadLook);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASpectateCharacter::MouseLook);
	}
}

void ASpectateCharacter::GamepadLook(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	const float WorldDeltaSecs = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	const float Yaw = LookAxis.X * (50 / 100) * WorldDeltaSecs; //todo get these from settings
	const float Pitch = LookAxis.Y * (50 / 100) * WorldDeltaSecs;

	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASpectateCharacter::MouseLook(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	const float Yaw = LookAxis.X * 50 / 100;//todo get these from settings
	const float Pitch = LookAxis.Y * 50 / 100;

	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("ASpectateCharacter::MouseLook controller null"));
	}
}

void ASpectateCharacter::Attach(APawn* Pawn)
{
	Server_Attach(Pawn);
}

void ASpectateCharacter::Server_Attach_Implementation(APawn* Pawn)
{
	const FAttachmentTransformRules Rules = FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,
		true
	);

	AttachToActor(Pawn, Rules, "head");
}
