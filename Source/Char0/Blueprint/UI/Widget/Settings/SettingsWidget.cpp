#include "SettingsWidget.h"

#include "CoreMinimal.h"
#include "Audio/AudioWidget.h"
#include "Controls/ControlsWidget.h"
#include "Graphics/GraphicsWidget.h"

void USettingsWidget::Back() const
{
	BackDelegate.Broadcast();
}

void USettingsWidget::ShowAudioWidget()
{
	Fade();

	UAudioWidget* Widget = GetAudioWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &USettingsWidget::BackAudio);
}

void USettingsWidget::ShowGraphicsWidget()
{
	Fade();

	UGraphicsWidget* Widget = GetGraphicsWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &USettingsWidget::BackGraphics);
}

void USettingsWidget::ShowControlsWidget()
{
	Fade();

	UControlsWidget* Widget = GetControlsWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &USettingsWidget::BackControls);
}

void USettingsWidget::BackAudio()
{
	UAudioWidget* Widget = GetAudioWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &USettingsWidget::BackAudio);

	Unfade();
}

void USettingsWidget::BackGraphics()
{
	UGraphicsWidget* Widget = GetGraphicsWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &USettingsWidget::BackGraphics);

	Unfade();
}

void USettingsWidget::BackControls()
{
	UControlsWidget* Widget = GetControlsWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &USettingsWidget::BackControls);

	Unfade();
}

UAudioWidget* USettingsWidget::GetAudioWidget()
{
	if (!AudioWidget)
	{
		AudioWidget = CreateWidget<UAudioWidget>(this, AudioWidgetClass);
		AudioWidget->AddToViewport();
	}
	return AudioWidget;
}

UGraphicsWidget* USettingsWidget::GetGraphicsWidget()
{
	if (!GraphicsWidget)
	{
		GraphicsWidget = CreateWidget<UGraphicsWidget>(this, GraphicsWidgetClass);
		GraphicsWidget->AddToViewport();
	}
	return GraphicsWidget;
}

UControlsWidget* USettingsWidget::GetControlsWidget()
{
	if (!ControlsWidget)
	{
		ControlsWidget = CreateWidget<UControlsWidget>(this, ControlsWidgetClass);
		ControlsWidget->AddToViewport();
	}
	return ControlsWidget;
}
