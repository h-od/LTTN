#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraversableActor.generated.h"

struct FTraversalCheckResult;
class USplineComponent;

UCLASS()
class CHAR0_API ATraversableActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<USplineComponent*> Ledges;

	UPROPERTY(BlueprintReadWrite)
	TMap<USplineComponent*, USplineComponent*> OppositeLedges;

public:
	ATraversableActor();
	
	UFUNCTION(BlueprintCallable) 
	void GetLedges(FVector HitLocation, FVector ActorLocation, UPARAM(ref) FTraversalCheckResult& ResultInOut);

protected:
	UFUNCTION(BlueprintCallable)
	void SetLedges(USplineComponent* Ledge_1, USplineComponent* Ledge_2, USplineComponent* Ledge_3, USplineComponent* Ledge_4);
	
private:
	UFUNCTION()
	USplineComponent* FindClosestLedgeToActor(const FVector& ActorLocation);
};
