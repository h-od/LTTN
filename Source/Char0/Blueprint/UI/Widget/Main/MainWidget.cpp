#include "MainWidget.h"

#include "MainNavigation.h"
#include "Char0/Blueprint/UI/Widget/Settings/SettingsWidget.h"
#include "Host/HostWidget.h"
#include "Join/JoinWidget.h"
#include "Start/StartWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ShowStart();
	APlayerController* PlayerController = GetOwningPlayer();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
}

void UMainWidget::FailedToJoin()
{
	GetJoinWidget()->SetButtonEnabled(false);
}

void UMainWidget::StartNavigation(const EMainNavigation Destination)
{
	switch (Destination)
	{
	case EMainNavigation::StartSolo:
		HideStart();
		ShowGameOnlyUI();
		StartSolo();
		break;
	case EMainNavigation::StartHost:
		HideHost();
		ShowGameOnlyUI();
		StartHost();
		break;
	case EMainNavigation::StartJoin:
		HideJoin();
		ShowGameOnlyUI();
		StartJoin(FText::FromString(""));
		break;

	case EMainNavigation::Host:
		HideStart();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainWidget::ShowHost, 0.5f, false);
		break;
	case EMainNavigation::Join:
		HideStart();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainWidget::ShowJoin, 0.5f, false);
		break;
	case EMainNavigation::Settings:
		HideStart();
		ShowSettings();
		break;
	case EMainNavigation::BackFromHost:
		HideHost();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainWidget::ShowStart, 0.5f, false);
		break;
	case EMainNavigation::BackFromJoin:
		HideJoin();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainWidget::ShowStart, 0.5f, false);
		break;
	}
}

void UMainWidget::ShowStart()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	UStartWidget* Widget = GetStartWidget();
	Widget->NavigationDelegate.BindUObject(this, &UMainWidget::StartNavigation);
	Widget->Unfade();
}

void UMainWidget::ShowHost()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	UHostWidget* Widget = GetHostWidget();
	Widget->NavigationDelegate.BindUObject(this, &UMainWidget::StartNavigation);
	Widget->Unfade();
}

void UMainWidget::ShowJoin()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	UJoinWidget* Widget = GetJoinWidget();
	Widget->NavigationDelegate.BindUObject(this, &UMainWidget::StartNavigation);
	Widget->Unfade();
}

void UMainWidget::ShowSettings()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	USettingsWidget* Widget = GetSettingsWidget();
	Widget->BackDelegate.AddDynamic(this, &UMainWidget::BackFromSettings);
	Widget->Unfade();
}

void UMainWidget::HideStart()
{
	UStartWidget* Widget = GetStartWidget();
	Widget->NavigationDelegate.Unbind();
	Widget->Fade();
}

void UMainWidget::HideHost()
{
	UHostWidget* Widget = GetHostWidget();
	Widget->NavigationDelegate.Unbind();
	Widget->Fade();
}

void UMainWidget::HideJoin()
{
	UJoinWidget* Widget = GetJoinWidget();
	Widget->NavigationDelegate.Unbind();
	Widget->Fade();
}

void UMainWidget::BackFromSettings()
{
	USettingsWidget* Widget = GetSettingsWidget();
	Widget->BackDelegate.RemoveDynamic(this, &UMainWidget::BackFromSettings);
	Widget->Fade();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainWidget::ShowStart, 0.5f, false);
}

void UMainWidget::ShowGameOnlyUI() const
{
	APlayerController* PlayerController = GetOwningPlayer();
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
}

UStartWidget* UMainWidget::GetStartWidget()
{
	if (!StartWidget)
	{
		StartWidget = CreateWidget<UStartWidget>(this, StartWidgetClass);
		StartWidget->AddToViewport();
	}
	return StartWidget;
}

UHostWidget* UMainWidget::GetHostWidget()
{
	if (!HostWidget)
	{
		HostWidget = CreateWidget<UHostWidget>(this, HostWidgetClass);
		HostWidget->AddToViewport();
	}
	return HostWidget;
}

UJoinWidget* UMainWidget::GetJoinWidget()
{
	if (!JoinWidget)
	{
		JoinWidget = CreateWidget<UJoinWidget>(this, JoinWidgetClass);
		JoinWidget->AddToViewport();
	}
	return JoinWidget;
}

USettingsWidget* UMainWidget::GetSettingsWidget()
{
	if (!SettingsWidget)
	{
		SettingsWidget = CreateWidget<USettingsWidget>(this, SettingsWidgetClass);
		SettingsWidget->AddToViewport();
	}
	return SettingsWidget;
}
