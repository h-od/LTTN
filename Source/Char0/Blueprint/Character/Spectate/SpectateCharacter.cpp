#include "SpectateCharacter.h"

#include "EnhancedInputComponent.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Controller/LttnController.h"
#include "Char0/Blueprint/GameMode/LttnGameMode.h"
#include "Kismet/GameplayStatics.h"

void ASpectateCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASpectateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(GamepadLookAction, ETriggerEvent::Triggered, this, &ASpectateCharacter::GamepadLook);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASpectateCharacter::MouseLook);
		EnhancedInputComponent->BindAction(PreviousAction, ETriggerEvent::Completed, this, &ASpectateCharacter::Previous);
		EnhancedInputComponent->BindAction(NextAction, ETriggerEvent::Completed, this, &ASpectateCharacter::Next);
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

void ASpectateCharacter::Previous()
{
	GetLttnController()->SpectatePrevious();
}

void ASpectateCharacter::Next()
{
	GetLttnController()->SpectateNext();
}

ALttnController* ASpectateCharacter::GetLttnController()
{
	if (!LttnController)
	{
		LttnController = Cast<ALttnController>(GetController());
	}
	return LttnController;
}

ALttnGameMode* ASpectateCharacter::GetLttnGameMode()
{
	if (!LttnGameMode)
	{
		LttnGameMode = Cast<ALttnGameMode>(GetWorld()->GetAuthGameMode());
	}

	return LttnGameMode;
}
