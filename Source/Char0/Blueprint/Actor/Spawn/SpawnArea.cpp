#include "SpawnArea.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Components/BillboardComponent.h"

ASpawnArea::ASpawnArea()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard"); 
	Billboard->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

FVector ASpawnArea::GetSpawnPoint() const
{
	const FVector SpawnPoint = GetSpawnPoint_Int();
	return FVector(SpawnPoint.X, SpawnPoint.Y, 196.0f); 
}

FVector ASpawnArea::GetSpawnPoint_Int() const
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return Billboard->GetComponentLocation();
	}

	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

	if (!NavSys)
	{
		return Billboard->GetComponentLocation();
	}

	FNavLocation Result;

	if (const bool bSuccess = NavSys->GetRandomPointInNavigableRadius(Billboard->GetComponentLocation(), 250.0f, Result); !bSuccess)
	{
		return Billboard->GetComponentLocation();
	}

	return Result.Location;
}
