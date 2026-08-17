#include "JoinWidget.h"

void UJoinWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}

void UJoinWidget::Start() const
{
	Parent->StartGame();
}

void UJoinWidget::Back() const
{
	Parent->ShowStartWidget();
}
