#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LttnSave.generated.h"

UCLASS()
class CHAR0_API ULttnSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FLinearColor Lights = FLinearColor::Green;
	UPROPERTY()
	FLinearColor Body = FLinearColor::Black;
	UPROPERTY()
	FLinearColor Joints = FLinearColor::Green;
};
