#include "CustomiseWidget.h"

#include "Char0/Blueprint/GameInstance/LttnGameInstance.h"

void UCustomiseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	GameInstance = Cast<ULttnGameInstance>(GetGameInstance());
}

void UCustomiseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetLights(GameInstance->Lights);
	SetBody(GameInstance->Body);
	SetJoints(GameInstance->Joints);
}

void UCustomiseWidget::UpdateLights(const FLinearColor Colour)
{
	GameInstance->SetLightsColour(Colour);
	SetLights(Colour);
}

void UCustomiseWidget::UpdateBody(const FLinearColor Colour)
{
	GameInstance->SetBodyColour(Colour);
	SetBody(Colour);
}

void UCustomiseWidget::UpdateJoints(const FLinearColor Colour)
{
	GameInstance->SetJointsColour(Colour);
	SetJoints(Colour);
}

void UCustomiseWidget::Back() const
{
	NavigationDelegate.ExecuteIfBound(EMainNavigation::BackFromCustomise);
}
