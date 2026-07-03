#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "StartController.generated.h"

class UStartWidget;

UCLASS()
class CHAR0_API AStartController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> StartWidgetClass;
	
public:
	AStartController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginDestroy() override;
};
