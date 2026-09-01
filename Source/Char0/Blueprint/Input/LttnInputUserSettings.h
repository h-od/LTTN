#pragma once

#include "CoreMinimal.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "LttnInputUserSettings.generated.h"

UCLASS()
class CHAR0_API ULttnInputUserSettings : public UEnhancedInputUserSettings
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, Config)
	float M_SensX = 0.8f;
	UPROPERTY(EditAnywhere, Config)
	float M_SensY = 0.8f;
	UPROPERTY(EditAnywhere, Config)
	float M_SensAimX = 0.4f;
	UPROPERTY(EditAnywhere, Config)
	float M_SensAimY = 0.4f;
	
	UPROPERTY(EditAnywhere, Config)
	float GP_SensX = 0.8f;
	UPROPERTY(EditAnywhere, Config)
	float GP_SensY = 0.8f;
	UPROPERTY(EditAnywhere, Config)
	float GP_SensAimX = 0.4f;
	UPROPERTY(EditAnywhere, Config)
	float GP_SensAimY = 0.4f;
	
	UPROPERTY(EditAnywhere, Config)
	bool bIsAiming = false;
	
	void SetMouseSensitivity(float X, float Y, float AimX, float AimY);
	
	FVector GetMouseSensitivityVector() const;
	
	void SetGamepadSensitivity(float X, float Y, float AimX, float AimY);
	
	FVector GetGamepadSensitivityVector() const;
	
	void SetIsAiming(bool bNewValue);
};
