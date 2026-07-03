#pragma once

#include "CoreMinimal.h"
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
	
	void CloseDoor();
	void OpenDoor();

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void Open();
	
	UFUNCTION(BlueprintImplementableEvent)
	void Close();
	
	UFUNCTION(BlueprintCallable)
	void OnDoorCloseComplete() const;
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void MC_CloseDoor();
	void MC_CloseDoor_Implementation();
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_OpenDoor();
	void MC_OpenDoor_Implementation();
};
