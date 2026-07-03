#pragma once

#include "CameraStyle.h"
#include "CameraMode.h"
#include "PropertiesForCamera.generated.h"

enum class ECameraMode : uint8;
enum class ECameraStyle : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FPropertiesForCamera
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECameraStyle CameraStyle = ECameraStyle::Medium;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECameraMode CameraMode = ECameraMode::Free;

	FPropertiesForCamera() = default;

	FPropertiesForCamera(const ECameraStyle Cs, const ECameraMode Cm)
	{
		CameraStyle = Cs;
		CameraMode = Cm;
	}
};
