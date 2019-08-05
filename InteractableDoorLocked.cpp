

#include "InteractableDoorLocked.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"

// Sets default values for this component's properties
UInteractableDoorLocked::UInteractableDoorLocked()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionType = EInteractionType::Unlockable;
}


// Called when the game starts
void UInteractableDoorLocked::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInteractableDoorLocked::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableDoorLocked::Interact()
{
	switch (LockType) 
	{
		case ELockType::KeyLock:
			if (bUnlocked)
			{
				ExecuteRotation();
			}
			else
			{
				// Play a sound of key locked door
			}
			break;

		case ELockType::LeverLock:
			if (!bUnlocked)
			{
				// Play a sound of lever locked door
			}
			break;
	}
}

void UInteractableDoorLocked::Activation()
{
	CheckLeversRequirement();
}

void UInteractableDoorLocked::CheckKeysRequirement(int PlayerKeys)
{
	//If true then unlock the door and open it
	if (PlayerKeys == NbOfKeysRequired)
	{
		bUnlocked = true;
		PlayUnlockSound();
		// See if it behaves weirdly
	}
}

void UInteractableDoorLocked::CheckLeversRequirement()
{
	int CurrentActivatedActors = 0;

	for (int i = 0; i< InteractableLeversToCheck.Num(); i++)
	{
		if (InteractableLeversToCheck[i]->GetbActivated() == true)
		{
			CurrentActivatedActors += 1;
		}
	}

	if (CurrentActivatedActors == RequiredNumberOfActivatedActors)
	{
		SetDirection(UGameplayStatics::GetPlayerPawn(this, 0));
		ExecuteRotation();
		GameMode->IncdoorOpened();
		GameMode->PlayAlarmSound(OwnerActor->GetActorLocation());
		UpdateLightColor();
	}
	else
	{
		if (Open == true)
		{
			SetDirection(UGameplayStatics::GetPlayerPawn(this, 0));
			ExecuteRotation();
			UpdateLightColor();
		}
	}


}

void UInteractableDoorLocked::PlayUnlockSound()
{
	// Play an unlocking sound when the door is unlocked
}

void UInteractableDoorLocked::UpdateLightColor()
{
	// Not sure what it does, but it is needed to call BP functions with C++
	FOutputDeviceNull ar;

	for (int i = 0; i < LightActorsToUpdate.Num(); i++)
	{
		if (Open)
		{
			// BP functions that update the light color of the point light and the material of SM_Cell_Light_02
			//LightActorsToUpdate[i]->CallFunctionByNameWithArguments(TEXT("SetMaterial"), ar, NULL, true);
			LightActorsToUpdate[i]->CallFunctionByNameWithArguments(TEXT("SetMaterial Green"), ar, NULL, true);
			LightActorsToUpdate[i]->CallFunctionByNameWithArguments(TEXT("SetLightColor Green"), ar, NULL, true);
		}
		else
		{
			// BP functions that update the light color of the point light and the material of SM_Cell_Light_02
			LightActorsToUpdate[i]->CallFunctionByNameWithArguments(TEXT("SetMaterial Red"), ar, NULL, true);
			LightActorsToUpdate[i]->CallFunctionByNameWithArguments(TEXT("SetLightColor Red"), ar, NULL, true);
		}
	}
}



