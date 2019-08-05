
// Interactive object that can be used to unlock doors and similar objects/objectives.
#pragma once

#include "Sound/SoundCue.h"
#include "ProjectJoolaGameModeBase.h"
#include "Components/PointLightComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Public/TimerManager.h"

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "InteractableSwitch.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Interactions), meta = (BlueprintSpawnableComponent))
class PROJECTJOOLA_API UInteractableSwitch : public UInteractableBase
{
	GENERATED_BODY()

public:

	UInteractableSwitch();

	UPROPERTY(EditAnywhere)
		TArray<AActor*> ActorsToInteract;

	UPROPERTY(EditAnywhere)
		float SwitchBeepingTimer =2.0f;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		//Determines if the lever has been activated or not.
		bool bActivated = false;

	UPROPERTY()
		UPointLightComponent* GreenLight;

	UPROPERTY()
		UPointLightComponent* RedLight;

	UFUNCTION()
		void SwitchOffTimerHandler();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* SoundSwitchOnOff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* SoundSwitchHumming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* SoundSwitchBeeping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* AudioComponentSwitchOnOff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* AudioComponentSwitchHumming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* AudioComponentSwitchBeeping;

	FTimerHandle SwitchOffTimerHandle;

	UFUNCTION()
		void PlaySoundOnOff();

	UFUNCTION()
		void PlaySoundHumming();

	UFUNCTION()
		void PlaySoundBeeping();

	virtual void Interact() override;

	// Reset function: Will be used later when the save/reload system is implement. Will handle the reset logic of the component's owner.
	//virtual void Reset() override;

	UFUNCTION()
		bool GetbActivated() { return bActivated; };
};
