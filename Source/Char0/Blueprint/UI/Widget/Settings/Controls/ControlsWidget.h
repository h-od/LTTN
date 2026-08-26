#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsWidget.generated.h"

class USettingsWidget;

UCLASS()
class CHAR0_API UControlsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
