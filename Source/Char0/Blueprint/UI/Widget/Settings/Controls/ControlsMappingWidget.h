#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsMappingWidget.generated.h"

struct FKeyMappingRow;
class UEnhancedInputUserSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMappingBackDelegate);

UCLASS()
class CHAR0_API UControlsMappingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FMappingBackDelegate BackDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnhancedInputUserSettings> Settings;
	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetControls(const TMap<FName, FKeyMappingRow>& PlayerKeyMappings);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetControlsNotFound();
	
	UFUNCTION(BlueprintCallable)
	void Back() const;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
};
