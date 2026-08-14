#include "MainWidget.h"

#include "Char0/Blueprint/UI/Widget/Settings/SettingsWidget.h"
#include "Host/HostWidget.h"
#include "Join/JoinWidget.h"
#include "Start/StartWidget.h"

void UMainWidget::SetStartWidget(UStartWidget* Widget)
{
	Widget->SetParent(this);
}

void UMainWidget::SetHostWidget(UHostWidget* Widget)
{
	Widget->SetParent(this);
}

void UMainWidget::SetJoinWidget(UJoinWidget* Widget)
{
	Widget->SetParent(this);
}

void UMainWidget::SetSettingsWidget(USettingsWidget* Widget)
{
	Widget->SetParent(this);
}
