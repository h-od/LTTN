#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.generated.h"

UCLASS()
class CHAR0_API ASpawnArea : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* Billboard;

public:
	UPROPERTY(EditInstanceOnly)
	int32 Index;

	UPROPERTY(EditInstanceOnly)
	bool bPlayer;

	ASpawnArea();

	FVector GetSpawnPoint() const;

private:
	FVector GetSpawnPoint_Int() const;
};
