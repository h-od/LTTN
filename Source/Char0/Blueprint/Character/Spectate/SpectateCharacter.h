#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/SpectatorPawn.h"
#include "SpectateCharacter.generated.h"

class ALttnCharacter;
class UInputAction;

UCLASS()
class CHAR0_API ASpectateCharacter : public ASpectatorPawn
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* GamepadLookAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MouseLookAction;
	
public:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void GamepadLook(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	
	void Attach(APawn* Pawn);
	
	UFUNCTION(Server, Reliable)
	void Server_Attach(APawn* Pawn);
	void Server_Attach_Implementation(APawn* Pawn);
};
