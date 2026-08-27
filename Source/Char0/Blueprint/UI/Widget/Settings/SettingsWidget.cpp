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
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USettingsWidget::UnfadeAudio, 0.5f, false);
}

void USettingsWidget::ShowGraphicsWidget()
{
	Fade();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USettingsWidget::UnfadeGraphics, 0.5f, false);
}

void USettingsWidget::ShowControlsWidget()
{
	Fade();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USettingsWidget::UnfadeControls, 0.5f, false);
}

void USettingsWidget::UnfadeAudio()
{
	UAudioWidget* Widget = GetAudioWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &USettingsWidget::BackAudio);
}

void USettingsWidget::UnfadeGraphics()
{
	UGraphicsWidget* Widget = GetGraphicsWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &USettingsWidget::BackGraphics);
}

void USettingsWidget::UnfadeControls()
{
	UControlsWidget* Widget = GetControlsWidget();
	Widget->Unfade();
	Widget->BackDelegate.AddDynamic(this, &USettingsWidget::BackControls);
}

void USettingsWidget::BackAudio()
{
	UAudioWidget* Widget = GetAudioWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &USettingsWidget::BackAudio);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USettingsWidget::Unfade, 0.5f, false);
}

void USettingsWidget::BackGraphics()
{
	UGraphicsWidget* Widget = GetGraphicsWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &USettingsWidget::BackGraphics);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USettingsWidget::Unfade, 0.5f, false);
}

void USettingsWidget::BackControls()
{
	UControlsWidget* Widget = GetControlsWidget();
	Widget->Fade();
	Widget->BackDelegate.RemoveDynamic(this, &USettingsWidget::BackControls);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USettingsWidget::Unfade, 0.5f, false);
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
