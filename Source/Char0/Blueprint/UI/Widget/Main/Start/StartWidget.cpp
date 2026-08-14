#include "StartWidget.h"

#include "CoreMinimal.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"

void UStartWidget::Solo() const
{
	Parent->StartSolo();
}

inline void UStartWidget::ShowSettings() const
{
	Parent->ShowSettings();
}

inline void UStartWidget::SetParent(UMainWidget* MainWidget)
{
	Parent = MainWidget;
}
