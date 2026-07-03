#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class ALttnCharacter;
enum class EInteractableType : uint8;

UCLASS()
class CHAR0_API AInteractable : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	EInteractableType Type;
	
public:
	AInteractable();
	
protected:
	UFUNCTION(BlueprintCallable)
	void NotifyActor(AActor* Actor, const bool bNotifyCan) const;

private:
	void Notify(ALttnCharacter* Character, const bool bNotifyCan) const;
};
