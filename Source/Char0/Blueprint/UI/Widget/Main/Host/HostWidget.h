#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "HostWidget.generated.h"

UCLASS()
class CHAR0_API UHostWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UMainWidget* Parent;

public:
	void SetParent(UMainWidget* MainWidget);
	
protected:
	UFUNCTION(BlueprintCallable)
	void StartGame() const;
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
