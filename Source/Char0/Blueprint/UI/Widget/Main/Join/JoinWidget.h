#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Char0/Blueprint/UI/Widget/Main/MainWidget.h"
#include "JoinWidget.generated.h"

UCLASS()
class CHAR0_API UJoinWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UMainWidget* Parent;
	
public:
	void SetParent(UMainWidget* MainWidget);
};
