#include "Tracker.h"

#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Controller/LttnController.h"
#include "Char0/Blueprint/GameMode/LttnGameMode.h"

void ATracker::BeginPlay()
{
	Super::BeginPlay();
	LttnGameMode = Cast<ALttnGameMode>(GetWorld()->GetAuthGameMode());
}

void ATracker::Notify(AActor* ActorPassed) const
{
	if (!HasAuthority())
	{
		return;
	}

	if (!ActorPassed->IsA(ALttnCharacter::StaticClass()))
	{
		return;
	}
	LttnGameMode->UpdatePlayerLocation(
		Cast<ALttnController>(Cast<ALttnCharacter>(ActorPassed)->GetController())->Id,
		Index
	);
}
