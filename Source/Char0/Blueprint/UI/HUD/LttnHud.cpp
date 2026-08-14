#include "LttnHud.h"

#include <string>

#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/Data/Player/PlayerManager.h"
#include "Char0/Blueprint/UI/Widget/FadeWidget.h"
#include "Char0/Blueprint/UI/Widget/Gameplay/OverlayWidget.h"
#include "Char0/Blueprint/UI/Widget/Gameplay/SpectatingWidget.h"

void ALttnHud::BeginPlay()
{
	Super::BeginPlay();
}

void ALttnHud::Initialise(const FPlayerManager& PlayerManager)
{
	UOverlayWidget* OverlayWidget = GetOverlay();
	OverlayWidget->SetWeaponProjectiles(FText::FromString(std::to_string(PlayerManager.Weapon.ProjectileCount).c_str()));
	OverlayWidget->SetMaxWeaponProjectiles(FText::FromString(std::to_string(PlayerManager.Weapon.MaxProjectiles).c_str()));
	OverlayWidget->SetPlayerHealth(1);
	OverlayWidget->SetPlayerStamina(1);
	OverlayWidget->SetScore(FText::FromString(std::to_string(PlayerManager.CurrentScore).c_str()));
	OverlayWidget->SetPlayerProjectiles(FText::FromString(std::to_string(PlayerManager.Player.Projectiles).c_str()));
	OverlayWidget->SetMaxPlayerProjectiles(FText::FromString(std::to_string(PlayerManager.Player.MaxProjectiles).c_str()));
	OverlayWidget->AddToViewport();
}

void ALttnHud::SetWave(const int32 Wave) const
{
	Overlay->SetWave(FText::FromString(std::to_string(Wave).c_str()));
}

void ALttnHud::SetHealth(const float Health) const
{
	Overlay->SetPlayerHealth(Health);
}

void ALttnHud::SetStamina(const float Health) const
{
	Overlay->SetPlayerStamina(Health);
}

void ALttnHud::SetWeaponProjectiles(const int32 Count) const
{
	Overlay->SetWeaponProjectiles(FText::FromString(std::to_string(Count).c_str()));
}

void ALttnHud::SetMaxWeaponProjectiles(const int32 Count) const
{
	Overlay->SetMaxWeaponProjectiles(FText::FromString(std::to_string(Count).c_str()));
}

void ALttnHud::SetPlayerProjectiles(const int32 Count) const
{
	Overlay->SetPlayerProjectiles(FText::FromString(std::to_string(Count).c_str()));
}

void ALttnHud::SetMaxPlayerProjectiles(const int32 Count) const
{
	Overlay->SetMaxPlayerProjectiles(FText::FromString(std::to_string(Count).c_str()));
}

void ALttnHud::ShowReloading(const int32 ReloadingTime) const
{
	Overlay->SetReloading(ReloadingTime);
}

void ALttnHud::SetScore(const int32 Score) const
{
	Overlay->SetScore(FText::FromString(std::to_string(Score).c_str()));
}

void ALttnHud::SetResupplyCooldown(const float RemainingTime) const
{
	const int32 Time = RemainingTime; //round it down
	const FString RemainingTimeString = std::to_string(Time).c_str();
	Overlay->SetResupplyCooldown(
		FText::FromString("Resupply in " + RemainingTimeString + "s")
	);
}

void ALttnHud::ShowInteract(const bool bShow) const
{
	Overlay->ShowInteract(bShow);
}

void ALttnHud::ShowSpectating(const bool bShow)
{
	if (bShow)
	{
		GetOverlay()->Hide();
		GetSpectate()->Show();
	}
	else
	{
		GetOverlay()->Show();
		GetSpectate()->Hide();
	}
}

UOverlayWidget* ALttnHud::GetOverlay()
{
	if (!Overlay)
	{
		Overlay = Cast<UOverlayWidget>(CreateWidget(GetWorld(), OverlayWidgetClass));
	}
	return Overlay;
}

UFadeWidget* ALttnHud::GetFade()
{
	if (!FadeWidget)
	{
		FadeWidget = Cast<UFadeWidget>(CreateWidget(GetWorld(), FadeWidgetClass));
	}
	return FadeWidget;
}

USpectatingWidget* ALttnHud::GetSpectate()
{
	if (!SpectatingWidget)
	{
		SpectatingWidget = Cast<USpectatingWidget>(CreateWidget(GetWorld(), SpectatingWidgetClass));
		SpectatingWidget->AddToViewport();
	}
	return SpectatingWidget;
}
