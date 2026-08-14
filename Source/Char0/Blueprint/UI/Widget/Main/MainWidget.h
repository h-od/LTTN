#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UJoinWidget;
class UHostWidget;
class USettingsWidget;
class UStartWidget;

UCLASS()
class CHAR0_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartSolo();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSettings();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowHost();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowJoin();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowStart();

protected:
	UFUNCTION(BlueprintCallable)
	void SetStartWidget(UStartWidget* Widget);
	
	UFUNCTION(BlueprintCallable)
	void SetHostWidget(UHostWidget* Widget);
	
	UFUNCTION(BlueprintCallable)
	void SetJoinWidget(UJoinWidget* Widget);
	
	UFUNCTION(BlueprintCallable)
	void SetSettingsWidget(USettingsWidget* Widget);

// private:
// 	UStartWidget* GetStartWidget();
// 	USettingsWidget* GetSettingsWidget();
};
