
// Interactive object that can be used to unlock doors and similar objects/objectives.

#include "InteractableSwitch.h"
#include "Engine/Engine.h"
#include "Misc/OutputDeviceNull.h"
#include "InteractableDoorLocked.h"
#include "Engine/EngineTypes.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"

UInteractableSwitch::UInteractableSwitch()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	AudioComponentSwitchOnOff = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentOnOff"));
	AudioComponentSwitchOnOff->bAutoActivate = false;

	AudioComponentSwitchHumming = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentHumming"));
	AudioComponentSwitchHumming->bAutoActivate = false;

	AudioComponentSwitchBeeping = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentBeeping"));
	AudioComponentSwitchBeeping->bAutoActivate = false;

	ObjectType = EObjectType::Switch;
}

void UInteractableSwitch::BeginPlay()
{
	Super::BeginPlay();

	// Attach audio components to the root of the object
	AudioComponentSwitchOnOff->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	AudioComponentSwitchOnOff->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	AudioComponentSwitchHumming->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	AudioComponentSwitchHumming->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	AudioComponentSwitchBeeping->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	AudioComponentSwitchBeeping->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Assigns a sound effect to the audio components
	if (AudioComponentSwitchOnOff->IsValidLowLevelFast())
		AudioComponentSwitchOnOff->SetSound(SoundSwitchOnOff);

	if (AudioComponentSwitchHumming->IsValidLowLevelFast())
		AudioComponentSwitchHumming->SetSound(SoundSwitchHumming);

	if (AudioComponentSwitchBeeping->IsValidLowLevelFast())
		AudioComponentSwitchBeeping->SetSound(SoundSwitchBeeping);

	// Sets up a timer for the beeping SE when the switch is off
	OwnerActor->GetWorldTimerManager().SetTimer(SwitchOffTimerHandle, this, &UInteractableSwitch::SwitchOffTimerHandler, SwitchBeepingTimer, true);
	OwnerActor->GetWorldTimerManager().PauseTimer(SwitchOffTimerHandle);

	// Get the light components from the actor and assign them to the right pointer
	TArray<UActorComponent*> lights = GetOwner()->GetComponentsByClass(UPointLightComponent::StaticClass());

	if (lights.Num() > 0)
	{
		if (lights[0]->ComponentHasTag("g"))
		{
			GreenLight = Cast<UPointLightComponent>(lights[0]);
			RedLight = Cast<UPointLightComponent>(lights[1]);
		}
		else
		{
			GreenLight = Cast<UPointLightComponent>(lights[1]);
			RedLight = Cast<UPointLightComponent>(lights[0]);
		}

	}

	// Initialize the switch with the state assigned through the editor
	if (bActivated)
	{
		RedLight->ToggleVisibility();
		OwnerActor->GetWorldTimerManager().PauseTimer(SwitchOffTimerHandle);
	}
	else
	{
		GreenLight->ToggleVisibility();
		OwnerActor->GetWorldTimerManager().UnPauseTimer(SwitchOffTimerHandle);
	}
}

void UInteractableSwitch::SwitchOffTimerHandler()
{
	PlaySoundBeeping();
}

// Play the on/off SE and generate an audio sphere
void UInteractableSwitch::PlaySoundOnOff()
{
	AudioComponentSwitchOnOff->Play();
	GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), 500);
}

void UInteractableSwitch::PlaySoundHumming()
{
	AudioComponentSwitchHumming->Play();
}

void UInteractableSwitch::PlaySoundBeeping()
{
	AudioComponentSwitchBeeping->Play();
}

void UInteractableSwitch::Interact()
{
	GreenLight->ToggleVisibility();
	RedLight->ToggleVisibility();
	PlaySoundOnOff();

	if (bActivated)
	{
		// Turn off the switch.
		// Change the light color.
		// Disable 'on' SE and enable the beeping SE timer.

		bActivated = false;
		AudioComponentSwitchHumming->FadeOut(1.0f, 0.0f);
		OwnerActor->GetWorldTimerManager().UnPauseTimer(SwitchOffTimerHandle);
	}
	else
	{
		// Turn on the switch.
		// Change the light color.
		// Disable beeping timer and enable the humming SE.

		bActivated = true;
		AudioComponentSwitchHumming->FadeIn(1.0f, SoundSwitchHumming->VolumeMultiplier);
		OwnerActor->GetWorldTimerManager().PauseTimer(SwitchOffTimerHandle);
	}

	// This code notifies the associated objects that the switch's state will affect.
	for (int i = 0; i < ActorsToInteract.Num(); i++)
	{
		// Casts UInteractableBase on the current ActorToInteract that may/may not be of type InteractableBase and then tries to Interact
		// If Success, the Activation will be called in the Interactable Component of the Actor
		// If Fail, nothing happens
		if (ActorsToInteract.Num() > 0)
			Cast<UInteractableBase>(ActorsToInteract[i]->FindComponentByClass<UInteractableBase>())->Activation();
	}
}
