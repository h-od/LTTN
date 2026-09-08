#include "IsAimingInputModifier.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "Char0/Blueprint/Input/LttnInputUserSettings.h"

FInputActionValue UIsAimingInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (!Settings)
	{
		Settings = PlayerInput->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->SetIsAiming(CurrentValue.Get<bool>()); 
	
	return Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
}
