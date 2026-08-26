#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "SettingsWidget.generated.h"

class UControlsWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBackDelegate);

UCLASS()
class CHAR0_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UControlsWidget> ControlsWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UControlsWidget> ControlsWidget;

public:
	FBackDelegate BackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;

	UFUNCTION(BlueprintCallable)
	void ShowControlsWidget() const;
	
	UFUNCTION()
	void ShowSettingsWidget() const;
};
