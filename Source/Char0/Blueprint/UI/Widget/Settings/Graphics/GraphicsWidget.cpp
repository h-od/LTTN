#include "GraphicsWidget.h"

void UGraphicsWidget::Back() const
{
	BackDelegate.Broadcast();
}
