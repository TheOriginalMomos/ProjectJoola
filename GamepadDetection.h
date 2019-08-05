

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "GamepadDetection.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJOOLA_API UGamepadDetection : public UObject
{

	GENERATED_BODY()

public:


	UGamepadDetection();
	~UGamepadDetection();

	/* Check if gamepad is connected at Runtime. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GamepadConnected"), Category = "System Information")
		static bool IsGamepadConnected();

};
