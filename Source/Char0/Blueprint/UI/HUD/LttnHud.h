#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LttnHud.generated.h"

class UStartWidget;
struct FPlayerManager;
class UOverlayWidget;
struct FPlayerStats;
class USpectatingWidget;
class UFadeWidget;

UCLASS()
class CHAR0_API ALttnHud : public AHUD
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlayWidget> OverlayWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStartWidget> StartWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFadeWidget> FadeWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USpectatingWidget> SpectatingWidgetClass;
	
	UPROPERTY()
	UOverlayWidget* Overlay;
	UPROPERTY()
	UFadeWidget* FadeWidget;
	UPROPERTY()
	USpectatingWidget* SpectatingWidget;

protected:
	virtual void BeginPlay() override;
	
public:
	void Initialise(const FPlayerManager& PlayerManager);
	void SetHealth(float Health) const;
	void SetStamina(float Health) const;
	void SetWeaponProjectiles(int32 Count) const;
	void SetMaxWeaponProjectiles(int32 Count) const;
	void SetPlayerProjectiles(int32 Count) const;
	void SetMaxPlayerProjectiles(int32 Count) const;
	void ShowReloading(int32 ReloadingTime) const;
	void SetScore(int32 Score) const;
	void SetResupplyCooldown(float RemainingTime) const;
	
	void ShowInteract(bool bShow) const;
	void ShowSpectating(bool bShow);
	void ShowCantStartGame();

private:
	UOverlayWidget* GetOverlay();
	UFadeWidget* GetFade();
	USpectatingWidget* GetSpectate();
};
