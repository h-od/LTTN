#include "ControlsMappingWidget.h"

#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UControlsMappingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningPlayer()->GetLocalPlayer()))
	{
		Settings = Subsystem->GetUserSettings();

		if (Settings)
		{
			TMap<FName, FKeyMappingRow> Tuples = Settings->GetActiveKeyProfile()->GetPlayerMappingRows();
			
			SetControls(Tuples);
		}
		else
		{
			SetControlsNotFound();
		}
	}
	else
	{
		SetControlsNotFound();
	}
}