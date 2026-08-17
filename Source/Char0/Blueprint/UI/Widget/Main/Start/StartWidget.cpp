#include "StartWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

void UStartWidget::Solo() const
{
	Parent->StartGame();
}

inline void UStartWidget::ShowHost() const
{
	Parent->ShowHostWidget();
}

inline void UStartWidget::ShowJoin() const
{
	Parent->ShowJoinWidget();
}

inline void UStartWidget::ShowSettings() const
{
	Parent->ShowSettingsWidget();
}

inline void UStartWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}
