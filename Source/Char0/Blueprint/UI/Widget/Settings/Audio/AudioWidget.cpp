#include "AudioWidget.h"

void UAudioWidget::Back() const
{
	BackDelegate.Broadcast();
}
