#include "Door.h"

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

void ADoor::OnDoorCloseComplete() const
{
	if (OnDoorCloseCompleteDelegate.IsBound())
	{
		OnDoorCloseCompleteDelegate.Broadcast();
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
