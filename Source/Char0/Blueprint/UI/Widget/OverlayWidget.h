#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

UCLASS()
class CHAR0_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerHealth(float Health);

	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerStamina(float Health);

	UFUNCTION(BlueprintImplementableEvent)
	void SetWeaponProjectiles(const FText& Count);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMaxWeaponProjectiles(const FText& Count);

	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerProjectiles(const FText& Count);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMaxPlayerProjectiles(const FText& Count);

	UFUNCTION(BlueprintImplementableEvent)
	void SetReloading(float ReloadTime);

	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(const FText& Score);

	UFUNCTION(BlueprintImplementableEvent)
	void SetResupplyCooldown(const FText& RemainingTime);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowInteract(const bool bShow);

	UFUNCTION(BlueprintImplementableEvent)
	void SetWave(const FText& Wave);

	void Show();
	void Hide();
};

inline void UOverlayWidget::Show()
{
	SetVisibility(ESlateVisibility::Visible);
}

inline void UOverlayWidget::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
