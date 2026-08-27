#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "SettingsWidget.generated.h"

class UGraphicsWidget;
class UAudioWidget;
class UControlsWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSettingsBackDelegate);

UCLASS()
class CHAR0_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAudioWidget> AudioWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGraphicsWidget> GraphicsWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UControlsWidget> ControlsWidgetClass;
	
private:
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	TObjectPtr<UAudioWidget> AudioWidget;
	UPROPERTY()
	TObjectPtr<UGraphicsWidget> GraphicsWidget;
	UPROPERTY()
	TObjectPtr<UControlsWidget> ControlsWidget;

public:
	FSettingsBackDelegate BackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
	UFUNCTION(BlueprintCallable)
	void ShowAudioWidget();
	UFUNCTION(BlueprintCallable)
	void ShowGraphicsWidget();
	UFUNCTION(BlueprintCallable)
	void ShowControlsWidget();

private:
	void UnfadeAudio();
	void UnfadeGraphics();
	void UnfadeControls();
	
	UFUNCTION()
	void BackAudio();
	UFUNCTION()
	void BackGraphics();
	UFUNCTION()
	void BackControls();

	UAudioWidget* GetAudioWidget();
	UGraphicsWidget* GetGraphicsWidget();
	UControlsWidget* GetControlsWidget();
};
