#include "HostWidget.h"

#include "Char0/Blueprint/UI/Widget/Main/Join/JoinWidget.h"

void UHostWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}

void UHostWidget::StartGame() const
{
	Parent->StartGame();
}

void UHostWidget::Back() const
{
	Parent->ShowStartWidget();
}
