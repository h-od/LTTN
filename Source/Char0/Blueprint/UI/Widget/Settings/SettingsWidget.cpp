#include "SettingsWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

void USettingsWidget::Back() const
{
	BackDelegate.Broadcast();
}

void USettingsWidget::ShowControlsWidget() const
{
}

void USettingsWidget::ShowSettingsWidget() const
{
}
