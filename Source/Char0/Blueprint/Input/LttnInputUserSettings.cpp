#include "LttnInputUserSettings.h"

void ULttnInputUserSettings::SetMouseSensitivity(const float X, const float Y, const float AimX, const float AimY)
{
	M_SensX = X;
	M_SensY = Y;
	M_SensAimX = AimX;
	M_SensAimY = AimY;

	ApplySettings();SaveSettings();
}

FVector ULttnInputUserSettings::GetMouseSensitivityVector() const
{
	if (bIsAiming)
	{
		return FVector(M_SensAimX, M_SensAimY, 1.0);
	}
	return FVector(M_SensX, M_SensY, 1.0);
}

void ULttnInputUserSettings::SetGamepadSensitivity(const float X, const float Y, const float AimX, const float AimY)
{
	GP_SensX = X;
	GP_SensY = Y;
	GP_SensAimX = AimX;
	GP_SensAimY = AimY;

	ApplySettings();SaveSettings();
}

FVector ULttnInputUserSettings::GetGamepadSensitivityVector() const
{
	if (bIsAiming)
	{
		return FVector(GP_SensAimX, GP_SensAimY, 1.0);
	}
	return FVector(GP_SensX, GP_SensY, 1.0);
}

void ULttnInputUserSettings::SetIsAiming(const bool bNewValue)
{
	bIsAiming = bNewValue;
	
	ApplySettings();
	SaveSettings();
}
