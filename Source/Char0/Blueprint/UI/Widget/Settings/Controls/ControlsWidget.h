#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsWidget.generated.h"

class UControlsMappingWidget;
class ULttnInputUserSettings;
class USettingsWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FControlsBackDelegate);

UCLASS()
class CHAR0_API UControlsWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	ULttnInputUserSettings* Settings;
	
	UPROPERTY()
	TObjectPtr<UControlsMappingWidget> MappingWidget;

public:
	FControlsBackDelegate BackDelegate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UControlsMappingWidget> MappingWidgetClass;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSensitivity(
		const float MouseX,
		const float MouseY,
		const float MouseAimingX,
		const float MouseAimingY,
		const float ControllerX,
		const float ControllerY,
		const float ControllerAimingX,
		const float ControllerAimingY
	);

	UFUNCTION(BlueprintCallable)
	void SetMouseX(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetMouseY(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetMouseAimingX(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetMouseAimingY(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetControllerX(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetControllerY(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetControllerAimingX(const float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetControllerAimingY(const float NewValue);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();

protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
	
	UFUNCTION(BlueprintCallable)
	void ShowMapping();
	UFUNCTION()
	void BackMapping();

private:
	UControlsMappingWidget* GetMappingWidget();
};
