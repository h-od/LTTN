#pragma once

#include "CoreMinimal.h"
#include "Char0/Blueprint/Actor/Interactable/Interactable.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorCloseCompleteDelegate);

UCLASS()
class CHAR0_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FOnDoorCloseCompleteDelegate OnDoorCloseCompleteDelegate;

	ADoor();
	
	UPROPERTY(EditInstanceOnly)
	int32 DoorNumber;
	UPROPERTY(EditInstanceOnly)
	int32 DoorLevel;
	
	void CloseDoor();
	void OpenDoor();

protected:
	
protected:
	UFUNCTION(BlueprintCallable)
	void NotifyActor(AActor* Actor, const bool bNotifyCan) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Open();
	
	UFUNCTION(BlueprintImplementableEvent)
	void Close();
	
	UFUNCTION(BlueprintCallable)
	void OnDoorCloseComplete() const;
	
private:
	void Notify(ALttnCharacter* Character, const bool bNotifyCan) const;
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_CloseDoor();
	void MC_CloseDoor_Implementation();
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_OpenDoor();
	void MC_OpenDoor_Implementation();
};
