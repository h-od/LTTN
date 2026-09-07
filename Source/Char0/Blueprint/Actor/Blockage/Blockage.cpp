#include "Blockage.h"

#include "Char0/Blueprint/Character/LttnCharacter.h"


ABlockage::ABlockage()
{
}

void ABlockage::OpenBlockage()
{
	MC_OpenBlockage();
}

void ABlockage::NotifyActor(AActor* Actor, const bool bNotifyCan) const
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

void ABlockage::Notify(ALttnCharacter* Character, const bool bNotifyCan) const
{
	if (bNotifyCan)
	{
		Character->CanOpenBlockage(BlockageNumber);
	}
	else
	{
		Character->CantInteract();
	}
}

void ABlockage::MC_OpenBlockage_Implementation()
{
	Destroy();
}
