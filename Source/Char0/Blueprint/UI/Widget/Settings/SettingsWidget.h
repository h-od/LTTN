#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "SettingsWidget.generated.h"

UCLASS()
class CHAR0_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWidget* Parent;

protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;

public:
	void SetParent(UMainWidget* MainWidget);
};
