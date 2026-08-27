#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioBackDelegate);

UCLASS()
class CHAR0_API UAudioWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FAudioBackDelegate BackDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
	UFUNCTION(BlueprintImplementableEvent)
	void Fade();
	
protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
