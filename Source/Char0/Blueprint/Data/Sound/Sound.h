#pragma once

#include "Sound.generated.h"

UENUM(BlueprintType)
enum class ESound : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Jump UMETA(DisplayName = "Jump"),
	Land UMETA(DisplayName = "Land"),
	Scuff UMETA(DisplayName = "Scuff"),
	Handplant UMETA(DisplayName = "Handplant"),
	RunBackwards UMETA(DisplayName = "RunBackwards"), 
	ScuffPivot UMETA(DisplayName = "ScuffPivot"),
	RunStrafe UMETA(DisplayName = "RunStrafe"),
	Tumble UMETA(DisplayName = "Tumble"),
	WalkBackwards UMETA(DisplayName = "WalkBackwards"),
	SlideLoop UMETA(DisplayName = "SlideLoop")
};
