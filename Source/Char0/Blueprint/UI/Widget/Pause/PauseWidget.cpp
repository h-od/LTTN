#include "PauseWidget.h"

#include "Char0/Blueprint/UI/Widget/Settings/SettingsWidget.h"

void UPauseWidget::ShowSettings()
{
	Fade();
	USettingsWidget* Widget = GetSettingsWidget();
	Widget->BackDelegate.AddDynamic(this, &UPauseWidget::BackFromSettings);
	Widget->Unfade();
}

void UPauseWidget::BackFromSettings()
{
	USettingsWidget* Widget = GetSettingsWidget();
	Widget->BackDelegate.RemoveDynamic(this, &UPauseWidget::BackFromSettings);
	Widget->Fade();
	Unfade();
}

USettingsWidget* UPauseWidget::GetSettingsWidget()
{
	if (!SettingsWidget)
	{
		SettingsWidget = CreateWidget<USettingsWidget>(this, SettingsWidgetClass);
		SettingsWidget->AddToViewport();
	}
	return SettingsWidget;
}
