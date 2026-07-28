#include "Door.h"

#include "Char0/Blueprint/Character/LttnCharacter.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADoor::CloseDoor()
{
	MC_CloseDoor();
}

void ADoor::OpenDoor()
{
	MC_OpenDoor();
}

void ADoor::NotifyActor(AActor* Actor, const bool bNotifyCan) const
{
	if (IsActorBeingDestroyed() or Actor->IsActorBeingDestroyed())
	{
		return;
	}
	if (Actor->IsA(ALttnCharacter::StaticClass()))
	{
		Notify(Cast<ALttnCharacter>(Actor), bNotifyCan);
	}
}

void ADoor::OnDoorCloseComplete() const
{
	if (OnDoorCloseCompleteDelegate.IsBound())
	{
		OnDoorCloseCompleteDelegate.Broadcast();
	}
}

void ADoor::Notify(ALttnCharacter* Character, const bool bNotifyCan) const
{
	if (bNotifyCan)
	{
		Character->CanOpenDoor(DoorNumber, DoorLevel);
	}
	else
	{
		Character->CantInteract();
	}
}

void ADoor::MC_CloseDoor_Implementation()
{
	Close();
}

void ADoor::MC_OpenDoor_Implementation()
{
	Open();
}
