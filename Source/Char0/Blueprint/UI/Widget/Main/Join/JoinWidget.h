#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "JoinWidget.generated.h"

DECLARE_DELEGATE_OneParam(FJoinNavDelegate, EMainNavigation);

UCLASS()
class CHAR0_API UJoinWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FJoinNavDelegate NavigationDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	UFUNCTION(BlueprintImplementableEvent)
	void SetButtonEnabled(bool bEnabled);

protected:
	UFUNCTION(BlueprintCallable)
	void JoinGame(const FText& JoinCode);
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
