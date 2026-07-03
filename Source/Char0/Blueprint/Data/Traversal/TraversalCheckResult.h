#pragma once
#include "TraversalActionType.h"

#include "TraversalCheckResult.generated.h"

enum class ETraversalActionType : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FTraversalCheckResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETraversalActionType ActionType = ETraversalActionType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasFrontLedge = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector FrontLedgeLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector FrontLedgeNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasBackLedge = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BackLedgeLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BackLedgeNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasBackFloor = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BackFloorLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ObstacleHeight = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ObstacleDepth = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BackLedgeHeight = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UPrimitiveComponent> HitComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> ChosenMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlayRate = 0.0f;

	FTraversalCheckResult() = default;

	FTraversalCheckResult(
		ETraversalActionType At,
		bool bHfl,
		FVector Fll,
		FVector Fln,
		bool bHbl,
		FVector Bll,
		FVector Bln,
		bool bHbf,
		FVector Bfl,
		float Oh,
		float Od,
		float Blh,
		TObjectPtr<UPrimitiveComponent> Hc,
		TObjectPtr<UAnimMontage> Cm,
		float St,
		float Pr
	)
	{
		ActionType = At;
		bHasFrontLedge = bHfl;
		FrontLedgeLocation = Fll;
		FrontLedgeNormal = Fln;
		bHasBackLedge = bHbl;
		BackLedgeLocation = Bll;
		BackLedgeNormal = Bln;
		bHasBackFloor = bHbf;
		BackFloorLocation = Bfl;
		ObstacleHeight = Oh;
		ObstacleDepth = Od;
		BackLedgeHeight = Blh;
		HitComponent = Hc;
		ChosenMontage = Cm;
		StartTime = St;
		PlayRate = Pr;
	}

	FString ToString() const
	{
		FString String = "ActionType: " + UEnum::GetValueAsString(ActionType) + "\n";

		if (bHasFrontLedge)
		{
			// String += "Front Ledge: ";
			String += "\t FrontLedgeLocation: " + FrontLedgeLocation.ToString() + "\n";
			// String += "\t FrontLedgeNormal: " + FrontLedgeNormal.ToString() + "\n";
		}
		else
		{
			String += "No Front Ledge";
		}

		// if (bHasBackLedge)
		// {
		// 	String += "Back Ledge: ";
		// 	String += "\t BackLedgeLocation: " + BackLedgeLocation.ToString() + "\n";
		// 	String += "\t BackLedgeNormal: " + BackLedgeNormal.ToString() + "\n";
		// }
		// else
		// {
		// 	String += "No Back Ledge";
		// }
		//
		// if (bHasBackFloor)
		// {
		// 	String += "Back Floor: ";
		// 	String += "\t BackFloorLocation: " + BackFloorLocation.ToString() + "\n";
		// 	String += "\t BackLedgeHeight: " + FString::SanitizeFloat(BackLedgeHeight) + "\n";
		// }
		// else
		// {
		// 	String += "No Back Floor";
		// }
		//
		//
		// String += "\n\t ObstacleHeight: " + FString::SanitizeFloat(ObstacleHeight) + "\n";
		// String += "\t ObstacleDepth: " + FString::SanitizeFloat(ObstacleDepth) + "\n";
		if (HitComponent)
		{
			String += "\t HitComponent: " + HitComponent->GetName() + "\n";
		}
		else
		{
			String += "No HitComponent";
		}
		return String;
	}
};
