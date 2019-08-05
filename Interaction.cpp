

#include "Interaction.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "PJCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/InputSettings.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UInteraction::UInteraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteraction::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	//PlayerHUD->ShowPlayerCharacterHUD();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Your fucking HUD is " + PlayerHUD->GetName()));

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		TArray<USceneComponent*> OwnerSceneComponents;
		OwnerCharacter->GetComponents<USceneComponent>(OwnerSceneComponents);
		for (int i = 0; i < OwnerSceneComponents.Num(); i++)
		{
			if (OwnerSceneComponents[i]->GetFName() == "HoldingComponent")
			{
				HoldLocation = OwnerSceneComponents[i];
				// Would normally set the relative location in player character class, but I get errors if I try changing anything in that class so it's there instead.
				HoldLocation->SetRelativeLocation(FVector(55.0f, 25.0f, -15.0f));
			}
		}

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("The HoldLocation is" + HoldLocation->GetName()));

		// Binds the Interaction action (E on the keyboard) to the Interaction's Interaction function
		OwnerCharacter->InputComponent->BindAction("Interaction", IE_Pressed, this, &UInteraction::Interaction);
		OwnerCharacter->InputComponent->BindAction("Release", IE_Pressed, this, &UInteraction::Release);
		OwnerCharacter->InputComponent->BindAction("Throw", IE_Pressed, this, &UInteraction::Throw);

		TArray<UCameraComponent*> OwnerCameras;
		OwnerCharacter->GetComponents<UCameraComponent>(OwnerCameras);
		OwnerMainCamera = OwnerCameras[0];

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Your character owner is " + OwnerCharacter->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Your character owner camera is " + OwnerMainCamera->GetName()));

	}

}


// Called every frame
void UInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (bHoldingObject)
	{
		UpdateGrabbedActorLocationAndRotation();
	}*/

	CheckForInteractable();
}

void UInteraction::CheckForInteractable()
{
	FVector ForwardVector = OwnerMainCamera->GetForwardVector();

	FVector StartTrace = OwnerMainCamera->GetComponentLocation();

	FVector EndTrace = StartTrace + (ForwardVector * InteractionDistance);

	FCollisionQueryParams CollisionParams;

	if (bDrawTrace)
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, -1.0f, 0, 5.0f);

	// Could change the ECC_Visibility Channel to the Interactable Channel if needed to be more clear about what channe
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, CollisionParams))
	{
		if (Hit.bBlockingHit)
		{
			// It seems to have fixed the crashing problem.
			InteractableActorCurrent = Cast<AActor>(Hit.GetActor());
			if (InteractableActorCurrent)
			{
				//UE_LOG(LogTemp, Warning, TEXT("The current actor you can interact with is %s"), (*InteractableActorCurrent->GetFName().ToString()));
				// Add a if to check if  NewInteractable == OldInteractable
				if (InteractableActorCurrent != ActorToGrabRelease)
				{
					if (InteractableActorCurrent != InteractableActorOld)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%s is not the same actor as the old one"), (*InteractableActorCurrent->GetFName().ToString()));
						//InteractableComponent = Cast<UInteractableBase>(InteractableActorCurrent->GetComponentByClass(UInteractableBase::StaticClass()));
						InteractableComponent = InteractableActorCurrent->FindComponentByClass<UInteractableBase>();
						if (InteractableComponent)
						{
							if (GEngine)
								GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("Custom component: " + InteractableComponent->GetName())));

							// Get the Interactable type from the actor via his component
							InteractableType = InteractableComponent->GetInteractionType();
							ObjectType = InteractableComponent->GetObjectType();

							switch (ObjectType)
							{
							case EObjectType::Door:
								break;
							case EObjectType::Lever:
								break;
							default:
								// Send the info to the UI to show custom Interaction message
								PlayerHUD->ShowPossibleInteraction(InteractableType);
								break;
							}

							if (ObjectType == EObjectType::Default)
							{
								// Send the info to the UI to show custom Interaction message
								PlayerHUD->ShowPossibleInteraction(InteractableType);
							}
							else
							{
								// Send the info to the UI to show custom Interaction message
								PlayerHUD->ShowPossibleInteraction(InteractableType, ObjectType, InteractableActorCurrent);
							}
							InteractableActorOld = InteractableActorCurrent;
						}
						else
						{
							//InteractableActorOld = nullptr;
							InteractableActorCurrent = nullptr;
							InteractableActorOld = nullptr;
							InteractableComponent = nullptr;
							InteractableType = EInteractionType::NoType;
							PlayerHUD->ShowPossibleInteraction(InteractableType);
						}
					}
					else
					{
						// Send the info to the UI to show custom Interaction message
						PlayerHUD->ShowPossibleInteraction(InteractableType, ObjectType, InteractableActorCurrent);
					}
				}
			}
			else
			{
				InteractableActorCurrent = nullptr;
				InteractableActorOld = nullptr;
				InteractableComponent = nullptr;
				InteractableType = EInteractionType::NoType;
				PlayerHUD->ShowPossibleInteraction(InteractableType);
			}
		}
	}
	else
	{
		/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, (TEXT("No there's no hit")));*/
		InteractableActorCurrent = nullptr;
		InteractableActorOld = nullptr;
		InteractableComponent = nullptr;
		InteractableType = EInteractionType::NoType;
		PlayerHUD->ShowPossibleInteraction(InteractableType);
	}
}

void UInteraction::Interaction()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("This is the Interaction function being activated by the press of the Interaction button")));

	// Add a switch to determine the type of interaction
	switch (InteractableType)
	{
		// Case Pickable : Call the Inventory gateway function to handle this one

	case EInteractionType::Unlockable:
		InteractableDoorLockedComponent = InteractableActorCurrent->FindComponentByClass<UInteractableDoorLocked>();
		if (InteractableDoorLockedComponent)
		{
			//Dummy variable. Will be replaced by actual proper call to inventory once it's implemented
			int PlayerKeys = 5;
			// Need to add some kind of verification to see the type of Lock it is. If it's lever related don't bother trying to unlock it else check the key requirements
			InteractableDoorLockedComponent->CheckKeysRequirement(PlayerKeys);
			InteractableDoorLockedComponent->Interact();
		}
		//		If door is unlocked then just call the regular Interact function
		//		Else call the Inventory to get either the number of keys or the array/vector of keys and send it to the door function that checks
		break;
	case EInteractionType::Throwable:
		// Call Grab function (doesn't exist atm) to grab the Throwable objects *Might be change to Throwable type for clarity reasons
		Grab();
		break;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("No corresponding Interactable type found"));
		// Check if it's nullptr and if it is then don't try to call the Interact function.
		if (InteractableComponent)
		{
			if (ObjectType == EObjectType::Door)
			{
				Cast<UInteractableDoor>(InteractableComponent)->SetDirection(OwnerCharacter);
			}
			InteractableComponent->Interact();
		}
		break;
	}

	// Default : Call the regular Interact function that all interactable components have
}

// Called when there's a Throwable actor to grab
void UInteraction::Grab()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("WE ARE GRABBING"));
	if (!bHoldingObject)
	{
		// If the current Interactable Actor is still valid
		if (InteractableActorCurrent)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, (TEXT("We are grabbing!")));
			ComponentToGrabRelease = Hit.GetComponent();
			ActorToGrabRelease = Hit.GetActor();

			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("HoldingComponent is at location : " + HoldLocation->GetComponentLocation().ToString())));

			ComponentToGrabRelease->SetSimulatePhysics(false);
			ActorToGrabRelease->AttachToComponent(HoldLocation, FAttachmentTransformRules::KeepRelativeTransform);
			ActorToGrabRelease->SetActorLocation(HoldLocation->GetComponentLocation());
			ActorToGrabRelease->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
			bHoldingObject = true;

			ComponentToGrabRelease->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			ComponentToGrabRelease->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

			// Might need to add a call to tell the object that he's beign held if the sounds produced by moving the object around is a problem

		}
	}
}

void UInteraction::Release()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("WE ARE RELEASING"));

	if (ComponentToGrabRelease)
	{
		// Release the object
		//HoldLocation->SetRelativeLocation(FVector(90.0f, 0.0f, 0.0f));
		ComponentToGrabRelease->SetSimulatePhysics(true);
		ComponentToGrabRelease->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		ComponentToGrabRelease->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		ComponentToGrabRelease->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		ActorToGrabRelease->SetActorScale3D(FVector(1.0f,1.0f,1.0f));
		ComponentToGrabRelease = nullptr;
		ActorToGrabRelease = nullptr;
		bHoldingObject = false;
	}
}

void UInteraction::Throw()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("WE ARE THROWING A GAME OF LOL AS USUAL"));

	if (ComponentToGrabRelease)
	{
		// Throw the object aka release and give an impulse to the object to send it forward.
		ComponentToGrabRelease->SetSimulatePhysics(true);
		FVector ForwardVector = OwnerMainCamera->GetForwardVector();

		ComponentToGrabRelease->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		ComponentToGrabRelease->AddImpulse(ForwardVector * ThrowStrength * ComponentToGrabRelease->GetMass());
		ComponentToGrabRelease->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		ComponentToGrabRelease->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		ActorToGrabRelease->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
		ComponentToGrabRelease = nullptr;
		ActorToGrabRelease = nullptr;
		bHoldingObject = false;
	}
}

void UInteraction::UpdateGrabbedActorLocationAndRotation()
{
	//Useless function, but will stay in code to avoid possibly breaking something so close to final presentation
	FRotator ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	HoldLocation->SetRelativeLocation(FVector(45.0f, 25.0f, -15.0f));
}

