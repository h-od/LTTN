#include "StartWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainNavigation.h"

void UStartWidget::Solo() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::StartSolo);
}

void UStartWidget::ShowHost() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::Host);
}

void UStartWidget::ShowJoin() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::Join);
}

void UStartWidget::ShowSettings() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::Settings);
}
