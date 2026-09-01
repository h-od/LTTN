#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "GamepadSensitivityInputModifier.generated.h"

class ULttnInputUserSettings;

UCLASS()
class CHAR0_API UGamepadSensitivityInputModifier : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
	
	UPROPERTY(Transient)
	ULttnInputUserSettings* Settings;
};
