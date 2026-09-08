#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LttnGameInstance.generated.h"

UCLASS()
class CHAR0_API ULttnGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 SaveSlotIndex = 0;
	UPROPERTY()
	FString SaveSlotName = "Save";
	
public:
	UPROPERTY(BlueprintReadOnly)
	FLinearColor Lights = FLinearColor::Green;
	UPROPERTY(BlueprintReadOnly)
	FLinearColor Body = FLinearColor::Black;
	UPROPERTY(BlueprintReadOnly)
	FLinearColor Joints = FLinearColor::Green;

protected:
	void Save();
	virtual void OnStart() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetLightsColour(FLinearColor NewValue);
	UFUNCTION(BlueprintCallable)
	void SetBodyColour(FLinearColor NewValue);
	UFUNCTION(BlueprintCallable)
	void SetJointsColour(FLinearColor NewValue);
};
