#include "HostWidget.h"

#include "Char0/Blueprint/UI/Widget/Main/MainNavigation.h"

void UHostWidget::StartGame() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::StartHost);
}

void UHostWidget::Back() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::BackFromHost);
}
