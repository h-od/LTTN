#include "JoinWidget.h"

#include "Char0/Blueprint/UI/Widget/Main/MainNavigation.h"

void UJoinWidget::JoinGame(const FText& JoinCode)
{
	SetButtonEnabled(false);
	NavigationDelegate.ExecuteIfBound(EMainNavigation::StartJoin);
}

void UJoinWidget::Back() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::BackFromJoin);
}
