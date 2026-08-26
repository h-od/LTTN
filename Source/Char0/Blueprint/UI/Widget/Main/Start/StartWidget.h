#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

enum class EMainNavigation : uint8;
class UMainWidget;

DECLARE_DELEGATE_OneParam(FStartNavDelegate, EMainNavigation);

UCLASS()
class CHAR0_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWidget* Parent;

public:
	FStartNavDelegate NavigationDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Solo() const;
	UFUNCTION(BlueprintCallable)
	void ShowHost() const;
	UFUNCTION(BlueprintCallable)
	void ShowJoin() const;
	UFUNCTION(BlueprintCallable)
	void ShowSettings() const;
};
