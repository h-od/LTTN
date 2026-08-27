#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGraphicsBackDelegate);

UCLASS()
class CHAR0_API UGraphicsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FGraphicsBackDelegate BackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
