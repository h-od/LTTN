#include "SettingsWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

void USettingsWidget::Back() const
{
	Parent->ShowStartWidget();
}

void USettingsWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}
