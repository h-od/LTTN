#pragma once

#include "SoundSide.h"
#include "SoundParams.generated.h"

enum class ESoundSide : uint8;

USTRUCT(BlueprintType)
struct CHAR0_API FSoundParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESoundSide Side = ESoundSide::None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Volume = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Pitch = 0.0f;
};
