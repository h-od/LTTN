#include "ControlsWidget.h"

void UControlsWidget::Back() const
{
	BackDelegate.Broadcast();
}
