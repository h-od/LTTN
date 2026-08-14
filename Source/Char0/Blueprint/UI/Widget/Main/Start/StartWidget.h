#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

class UMainWidget;

UCLASS()
class CHAR0_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWidget* Parent;

protected:
	UFUNCTION(BlueprintCallable)
	void Solo() const;
	UFUNCTION(BlueprintCallable)
	void ShowSettings() const;

public:
	void SetParent(UMainWidget* MainWidget);
};
