#include "Interactable.h"

#include "Char0/Blueprint/Character/LttnCharacter.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractable::NotifyActor(AActor* Actor, const bool bNotifyCan) const
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

void AInteractable::Notify(ALttnCharacter* Character, const bool bNotifyCan) const
{
	if (bNotifyCan)
	{
		Character->CanInteract(Type);
	}
	else
	{
		Character->CantInteract();
	}
}
