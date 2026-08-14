#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSummaryWidget.generated.h"

UCLASS()
class CHAR0_API UGameSummaryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSummary(const FText& Summary);
};
