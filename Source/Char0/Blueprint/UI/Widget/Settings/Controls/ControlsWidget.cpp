#include "ControlsWidget.h"

#include "ControlsMappingWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Char0/Blueprint/Input/LttnInputUserSettings.h"

void UControlsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}

	SetSensitivity(
		Settings->M_SensX,
		Settings->M_SensY,
		Settings->M_SensAimX,
		Settings->M_SensAimY,
		Settings->GP_SensX,
		Settings->GP_SensY,
		Settings->GP_SensAimX,
		Settings->GP_SensAimY
	);
}

void UControlsWidget::SetMouseX(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->M_SensX = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetMouseY(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->M_SensY = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetMouseAimingX(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->M_SensAimX = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetMouseAimingY(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->M_SensAimY = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetControllerX(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->GP_SensX = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetControllerY(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->GP_SensY = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetControllerAimingX(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->GP_SensAimX = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::SetControllerAimingY(const float NewValue)
{
	if (!Settings)
	{
		Settings = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->GetUserSettings<ULttnInputUserSettings>();
	}
	Settings->GP_SensAimY = NewValue;
	Settings->ApplySettings();
	Settings->SaveSettings();
}

void UControlsWidget::Back() const
{
	BackDelegate.Broadcast();
}

void UControlsWidget::ShowMapping() 
{
	UControlsMappingWidget* Widget = GetMappingWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &UControlsWidget::BackMapping);

	Fade();
}

void UControlsWidget::BackMapping() 
{
	UControlsMappingWidget* Widget = GetMappingWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &UControlsWidget::BackMapping);

	Unfade();
}

UControlsMappingWidget* UControlsWidget::GetMappingWidget()
{
	if (!MappingWidget)
	{
		MappingWidget = CreateWidget<UControlsMappingWidget>(this, MappingWidgetClass);
		MappingWidget->AddToViewport();
	}
	return MappingWidget;
}
