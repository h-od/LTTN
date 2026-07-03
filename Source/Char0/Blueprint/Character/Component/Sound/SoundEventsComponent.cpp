#include "SoundEventsComponent.h"

#include "GameplayTagContainer.h"
#include "Char0/Blueprint/Data/Sound/SoundParams.h"
#include "Kismet/GameplayStatics.h"

USoundEventsComponent::USoundEventsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
}

void USoundEventsComponent::PlaySound(const FGameplayTag& Tag, const FSoundParams& Params)
{
	USoundBase* Sound = Find(Tag);
	
	if (!Sound)
	{
		return;
	}

	UGameplayStatics::SpawnSoundAttached(
		Sound,
		GetOwner()->GetRootComponent(),
		FName("None"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		false,
		Params.Volume,
		Params.Pitch,
		0.0f,
		nullptr,
		nullptr,
		true
	);
}

USoundBase* USoundEventsComponent::Find(const FGameplayTag& Tag)
{
	if (AudioBank.Contains(Tag))
	{
		return AudioBank[Tag];
	}
	return nullptr;
}
