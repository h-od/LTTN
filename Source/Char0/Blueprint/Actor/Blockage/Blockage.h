#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blockage.generated.h"

class ALttnCharacter;

UCLASS()
class CHAR0_API ABlockage : public AActor
{
	GENERATED_BODY()

public:
	ABlockage();
	
	UPROPERTY(EditInstanceOnly)
	int32 BlockageNumber;
	
	void OpenBlockage();

protected:
	UFUNCTION(BlueprintCallable)
	void NotifyActor(AActor* Actor, const bool bNotifyCan) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Open();
	
private:
	void Notify(ALttnCharacter* Character, const bool bNotifyCan) const;
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_OpenBlockage();
	void MC_OpenBlockage_Implementation();
};
