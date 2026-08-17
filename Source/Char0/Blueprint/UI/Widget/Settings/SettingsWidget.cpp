#include "SettingsWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

inline void USettingsWidget::Back() const
{
	Parent->ShowStartWidget();
}

inline void USettingsWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}
