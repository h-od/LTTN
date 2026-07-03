#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SoundEventsComponent.generated.h"

struct FSoundParams;

UCLASS()
class CHAR0_API USoundEventsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, USoundBase*> AudioBank;
	
public:
	USoundEventsComponent();

	UFUNCTION(BlueprintCallable)
	void PlaySound(const FGameplayTag& Tag, const FSoundParams& Params);
	
private:
	UFUNCTION()
	USoundBase* Find(const FGameplayTag& Tag);
};
