#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PreTickComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTick);

UCLASS()
class CHAR0_API UPreTickComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPreTickComponent();
	UPROPERTY()
	FTick OnTick;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
