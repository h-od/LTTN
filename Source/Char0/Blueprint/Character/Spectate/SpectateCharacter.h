#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/SpectatorPawn.h"
#include "SpectateCharacter.generated.h"

class ALttnGameMode;
class ALttnController;
class USpectatingWidget;
class ALttnCharacter;
class UInputAction;

UCLASS()
class CHAR0_API ASpectateCharacter : public ASpectatorPawn
{
	GENERATED_BODY()
	
	UPROPERTY()
	ALttnGameMode* LttnGameMode;
	UPROPERTY()
	ALttnController* LttnController;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* GamepadLookAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* PreviousAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* NextAction;
	
public:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void GamepadLook(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);

private:
	void Previous();
	void Next();
	
	ALttnController* GetLttnController();
	ALttnGameMode* GetLttnGameMode();
};
