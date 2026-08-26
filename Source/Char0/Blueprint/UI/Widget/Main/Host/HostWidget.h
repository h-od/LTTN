#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "HostWidget.generated.h"

DECLARE_DELEGATE_OneParam(FHostNavDelegate, EMainNavigation);

UCLASS()
class CHAR0_API UHostWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FHostNavDelegate NavigationDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void StartGame() const;
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
