#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainNavigation.h"
#include "CustomiseWidget.generated.h"

class ULttnGameInstance;
DECLARE_DELEGATE_OneParam(FCustomiseNavDelegate, EMainNavigation);

UCLASS()
class CHAR0_API UCustomiseWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	ULttnGameInstance* GameInstance;
	
public:
	FCustomiseNavDelegate NavigationDelegate;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetLights(FLinearColor Colour);
	UFUNCTION(BlueprintImplementableEvent)
	void SetBody(FLinearColor Colour);
	UFUNCTION(BlueprintImplementableEvent)
	void SetJoints(FLinearColor Colour);

	UFUNCTION(BlueprintCallable)
	void UpdateLights(FLinearColor Colour);
	UFUNCTION(BlueprintCallable)
	void UpdateBody(FLinearColor Colour);
	UFUNCTION(BlueprintCallable)
	void UpdateJoints(FLinearColor Colour);

public:	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
