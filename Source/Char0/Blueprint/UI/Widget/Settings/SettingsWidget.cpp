#include "SettingsWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

void USettingsWidget::Back() const
{
	Parent->ShowStartWidget();
}

void USettingsWidget::ShowControlsWidget() const
{
	Parent->ShowControlsWidget();
}

void USettingsWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}

void USettingsWidget::ShowSettingsWidget() const
{
	Parent->BackFromControls();
}
