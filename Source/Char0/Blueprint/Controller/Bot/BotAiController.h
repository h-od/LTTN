#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotAiController.generated.h"

UCLASS()
class CHAR0_API ABotAiController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void EnableStateTree();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DisableStateTree();
};
