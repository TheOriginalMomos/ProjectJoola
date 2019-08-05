
// Inherited class used by the engine for camera shakes.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "HeadBobbing.generated.h"

/**
 *
 */
UCLASS()
class PROJECTJOOLA_API UHeadBobbing : public UCameraShake
{
	GENERATED_BODY()

public:

	UHeadBobbing();

};
