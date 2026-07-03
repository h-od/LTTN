#include "TraversableActor.h"

#include "Char0/Blueprint/Data/Traversal/TraversalCheckResult.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATraversableActor::ATraversableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATraversableActor::SetLedges(USplineComponent* Ledge_1, USplineComponent* Ledge_2, USplineComponent* Ledge_3, USplineComponent* Ledge_4)
{
	Ledges.Empty();
	Ledges.Add(Ledge_1);
	Ledges.Add(Ledge_2);
	Ledges.Add(Ledge_3);
	Ledges.Add(Ledge_4);

	OppositeLedges.Empty();
	OppositeLedges.Add(Ledge_1, Ledge_2);
	OppositeLedges.Add(Ledge_2, Ledge_1);
	OppositeLedges.Add(Ledge_3, Ledge_4);
	OppositeLedges.Add(Ledge_4, Ledge_3);
}

void ATraversableActor::GetLedges(FVector HitLocation, FVector ActorLocation, FTraversalCheckResult& ResultInOut)
{
	const USplineComponent* ClosestLedge = FindClosestLedgeToActor(ActorLocation);
	constexpr float MinLedgeWidth = 60.0;
	if (ClosestLedge == nullptr)
	{
		ResultInOut.bHasFrontLedge = false;
		return;
	}
	if (ClosestLedge->GetSplineLength() < MinLedgeWidth)
	{
		ResultInOut.bHasFrontLedge = false;
		return;
	}

	const float Distance = ClosestLedge->GetDistanceAlongSplineAtLocation(
		ClosestLedge->FindLocationClosestToWorldLocation(HitLocation, ESplineCoordinateSpace::Local),
		ESplineCoordinateSpace::Local
	);
	constexpr float HalfMinLedgeWidth = MinLedgeWidth / 2;

	const float DistanceClamped = UKismetMathLibrary::FClamp(
		Distance,
		HalfMinLedgeWidth,
		ClosestLedge->GetSplineLength() - HalfMinLedgeWidth
	);

	const FTransform FrontLedgeTransform = ClosestLedge->GetTransformAtDistanceAlongSpline(DistanceClamped, ESplineCoordinateSpace::World, false);
	const FVector FrontLedgeLocation = FrontLedgeTransform.GetLocation();

	ResultInOut.bHasFrontLedge = true;
	ResultInOut.FrontLedgeLocation = FrontLedgeLocation;
	ResultInOut.FrontLedgeNormal = UKismetMathLibrary::GetUpVector(FrontLedgeTransform.Rotator());

	const USplineComponent* OppositeLedge = OppositeLedges[ClosestLedge];

	if (!OppositeLedge)
	{
		ResultInOut.bHasBackLedge = false;
		return;
	}

	const FTransform BackLedgeTransform = OppositeLedge->FindTransformClosestToWorldLocation(FrontLedgeLocation, ESplineCoordinateSpace::World, false);
	const FVector BackLedgeLocation = BackLedgeTransform.GetLocation();

	ResultInOut.bHasBackLedge = true;
	ResultInOut.BackLedgeLocation = BackLedgeLocation;
	ResultInOut.BackLedgeNormal = UKismetMathLibrary::GetUpVector(BackLedgeTransform.Rotator());
}

USplineComponent* ATraversableActor::FindClosestLedgeToActor(const FVector& ActorLocation)
{
	if (Ledges.IsEmpty())
	{
		return nullptr;
	}

	float Closest = -1;
	USplineComponent* ClosestLedge = nullptr;
	for (USplineComponent* Ledge : Ledges)
	{
		const FVector LocationClosest = Ledge->FindLocationClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World);
		const FVector UpLocationClosest = Ledge->FindUpVectorClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World);

		const float Distance = UKismetMathLibrary::Vector_Distance(
			LocationClosest + UKismetMathLibrary::Multiply_VectorFloat(UpLocationClosest, 10.0),
			ActorLocation
		);

		// if (ClosestLedge == nullptr)
		if (Closest == -1)
		{
			Closest = Distance;
			ClosestLedge = Ledge;
		}
		else
		{
			if (Distance < Closest)
			{
				Closest = Distance;
				ClosestLedge = Ledge;
			}
		}
	}
	return ClosestLedge;
}