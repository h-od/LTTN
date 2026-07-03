#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tracker.generated.h"

class ALttnGameMode;

UCLASS()
class CHAR0_API ATracker : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	ALttnGameMode* LttnGameMode;
	
protected:
	UPROPERTY(EditInstanceOnly)
	int32 Index;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void Notify(AActor* ActorPassed) const;
};