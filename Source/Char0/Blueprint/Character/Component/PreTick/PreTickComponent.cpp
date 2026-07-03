#include "PreTickComponent.h"

UPreTickComponent::UPreTickComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
}

void UPreTickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnTick.Broadcast();
}
