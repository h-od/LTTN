#include "StartController.h"

#include "Blueprint/UserWidget.h"

AStartController::AStartController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStartController::BeginPlay()
{
	Super::BeginPlay();
	CreateWidget(GetWorld(), StartWidgetClass)->AddToViewport();
	SetInputMode(FInputModeGameAndUI());
}

void AStartController::BeginDestroy()
{
	Super::BeginDestroy();
	SetInputMode(FInputModeGameOnly());
}
