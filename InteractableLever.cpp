// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableLever.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "InteractableDoorLocked.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "EngineUtils.h"

UInteractableLever::UInteractableLever()
{
	PrimaryComponentTick.bCanEverTick = true;

	DownAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Down"));
	DownAudioComponent->bAutoActivate = false;

	UpAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Up"));
	UpAudioComponent->bAutoActivate = false;

	ObjectType = EObjectType::Lever;
}

// Called when the game starts
void UInteractableLever::BeginPlay()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, (TEXT("InteractableLever")));

	Super::BeginPlay();

	DownAudioComponent->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	DownAudioComponent->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
	UpAudioComponent->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	UpAudioComponent->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));

	if (DownAudioComponent->IsValidLowLevelFast())
	{
		DownAudioComponent->SetSound(DownSound);
	}
	if (UpAudioComponent->IsValidLowLevelFast())
	{
		UpAudioComponent->SetSound(UpSound);
	}

}

void UInteractableLever::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractableLever::PlaySoundDown()
{
	DownAudioComponent->Play();
	GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), 500);
}

void UInteractableLever::PlaySoundUp()
{
	UpAudioComponent->Play();
	GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), 500);
}

void UInteractableLever::Interact()
{
	// Not sure what it does, but it is needed to call BP functions with C++
	FOutputDeviceNull ar;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("We are in InteractableLever Interact()")));

	if (!bAnimationIsPlaying)
	{
		if (!bActivated)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("The lever is now down")));
			bActivated = true;
			// Play lever animation - Down part
			OwnerActor->CallFunctionByNameWithArguments(TEXT("PlayDownAnimation"), ar, NULL, true);
			// Play lever sound - Down part
			PlaySoundDown();

		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("The lever is now up")));
			bActivated = false;

			// Play level animation - Up part
			OwnerActor->CallFunctionByNameWithArguments(TEXT("PlayUpAnimation"), ar, NULL, true);
			// Play lever sound - Up part
			PlaySoundUp();
		}

		// Loop through the array of actors to interact and calls the Interact function of each actor.
		for (int i = 0; i < ActorsToInteract.Num(); i++)
		{
			//ActorsToInteract.FindComponentByClass<UInteractableBase>();
			// Casts UInteractableBase on the current ActorToInteract that may/may not be of type InteractableBase and then tries to Interact
			// If Success, the Activation will be called in the Interactable Component of the Actor
			// If Fail, nothing happens
			if (ActorsToInteract.Num() > 0)
				Cast<UInteractableBase>(ActorsToInteract[i]->FindComponentByClass<UInteractableBase>())->Activation();
		}
	}
}
