#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsWidget.generated.h"

class USettingsWidget;

UCLASS()
class CHAR0_API UControlsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	USettingsWidget* Parent;

protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
	
	UFUNCTION(BlueprintCallable)
	void SetParent(USettingsWidget* SettingsWidget);
};
