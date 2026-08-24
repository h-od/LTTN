#include "StartWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

void UStartWidget::Solo() const
{
	Parent->StartGame();
}

void UStartWidget::ShowHost() const
{
	Parent->ShowHostWidget();
}

void UStartWidget::ShowJoin() const
{
	Parent->ShowJoinWidget();
}

void UStartWidget::ShowSettings() const
{
	Parent->ShowSettingsWidget();
}

void UStartWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}
