#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UCustomiseWidget;
enum class EMainNavigation : uint8;
class UJoinWidget;
class UHostWidget;
class USettingsWidget;
class UStartWidget;
class ULoadingWidget;

UCLASS()
class CHAR0_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStartWidget> StartWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHostWidget> HostWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UJoinWidget> JoinWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCustomiseWidget> CustomiseWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USettingsWidget> SettingsWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UStartWidget> StartWidget;
	UPROPERTY()
	TObjectPtr<UHostWidget> HostWidget;
	UPROPERTY()
	TObjectPtr<UJoinWidget> JoinWidget;
	UPROPERTY()
	TObjectPtr<UCustomiseWidget> CustomiseWidget;
	UPROPERTY()
	TObjectPtr<USettingsWidget> SettingsWidget;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartSolo();
	UFUNCTION(BlueprintImplementableEvent)
	void StartHost();
	UFUNCTION(BlueprintImplementableEvent)
	void StartJoin(const FText& Code);

	UFUNCTION(BlueprintCallable)
	void FailedToJoin();
	
private:
	UFUNCTION()
	void StartNavigation(EMainNavigation Destination);
	
	void ShowStart();
	void ShowSettings();
	void ShowJoin();
	void ShowCustomise();
	void ShowHost();
	
	void HideStart();
	void HideHost();
	void HideJoin();
	void HideCustomise();
	UFUNCTION()
	void BackFromSettings();

	void ShowGameOnlyUI() const;
	
	UStartWidget* GetStartWidget();
	UHostWidget* GetHostWidget();
	UJoinWidget* GetJoinWidget();
	UCustomiseWidget* GetCustomiseWidget();
	USettingsWidget* GetSettingsWidget();
};
