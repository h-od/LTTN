#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsMappingWidget.generated.h"

struct FKeyMappingRow;
class UEnhancedInputUserSettings;

UCLASS()
class CHAR0_API UControlsMappingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnhancedInputUserSettings> Settings;
	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetControls(const TMap<FName, FKeyMappingRow>& PlayerKeyMappings);
	UFUNCTION(BlueprintImplementableEvent)
	void SetControlsNotFound();
};
