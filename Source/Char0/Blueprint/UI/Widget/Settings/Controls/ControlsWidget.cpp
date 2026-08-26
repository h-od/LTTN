#include "ControlsWidget.h"

#include "Char0/Blueprint/UI/Widget/Settings/SettingsWidget.h"

void UControlsWidget::Back() const
{
	Parent->ShowSettingsWidget();
}

void UControlsWidget::SetParent(USettingsWidget* SettingsWidget)
{
	Parent = SettingsWidget;
}
