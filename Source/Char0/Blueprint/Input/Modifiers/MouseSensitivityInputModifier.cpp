#include "MouseSensitivityInputModifier.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "Char0/Blueprint/Input/LttnInputUserSettings.h"

FInputActionValue UMouseSensitivityInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (!Settings)
	{
		Settings = PlayerInput->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	
	return CurrentValue.Get<FVector>() * Settings->GetMouseSensitivityVector();
}
