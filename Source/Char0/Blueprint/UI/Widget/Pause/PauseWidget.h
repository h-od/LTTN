#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class USettingsWidget;

UCLASS()
class CHAR0_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USettingsWidget> SettingsWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<USettingsWidget> SettingsWidget;

	UPROPERTY()
	FTimerHandle TimerHandle;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();

	UFUNCTION(BlueprintCallable)
	void ShowSettings();

private:	
	UFUNCTION()
	void BackFromSettings();

	USettingsWidget* GetSettingsWidget();
};
