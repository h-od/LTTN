#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectatingWidget.generated.h"

UCLASS()
class CHAR0_API USpectatingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	void Hide();
};

inline void USpectatingWidget::Show()
{
	SetVisibility(ESlateVisibility::Visible);
}

inline void USpectatingWidget::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
