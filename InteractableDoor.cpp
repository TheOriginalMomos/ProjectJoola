

#include "InteractableDoor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties

UInteractableDoor::UInteractableDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TimelineCallback.BindUFunction(this, FName("UpdateActorRotation"));
	TimelineFinishedCallback.BindUFunction(this, FName("SetReadyState"));

	OpeningAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Open"));
	OpeningAudioComponent->bAutoActivate = false;
	//OpeningAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	ClosingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Close"));
	ClosingAudioComponent->bAutoActivate = false;
	//ClosingAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	SmashingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Smash"));
	SmashingAudioComponent->bAutoActivate = false;

	ObjectType = EObjectType::Door;
}

void UInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> OwnerStaticMeshComponents;
	OwnerActor->GetComponents<UStaticMeshComponent>(OwnerStaticMeshComponents);

	for (int i = 0; i < OwnerStaticMeshComponents.Num(); i++)
	{
		UStaticMeshComponent* TemporaryElement = OwnerStaticMeshComponents[i];

		// Check if it's possible to set the Door in editor instead of having to use that meh function
		if (TemporaryElement->GetFName() == "Door")
		{
			ActorToRotate = TemporaryElement;
		}
	}

	TArray<USceneComponent*> OwnerSceneComponents;
	OwnerActor->GetComponents<USceneComponent>(OwnerSceneComponents);

	//TArray<UActorComponent*> OwnerSceneComponents;
	//OwnerSceneComponents = OwnerActor->GetComponentsByClass(USceneComponent::StaticClass());

	for (int i = 0; i < OwnerSceneComponents.Num(); i++)
	{
		USceneComponent* TemporaryElement = OwnerSceneComponents[i];

		if (TemporaryElement->GetFName() == "SoundOrigin")
		{
			SoundLocation = TemporaryElement;
		}
	}

	OpeningAudioComponent->AttachToComponent(ActorToRotate, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OpeningAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ClosingAudioComponent->AttachToComponent(ActorToRotate, FAttachmentTransformRules::SnapToTargetIncludingScale);
	ClosingAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	if (OpeningAudioComponent->IsValidLowLevelFast()) 
	{
		OpeningAudioComponent->SetSound(OpeningSound);
	}
	if (ClosingAudioComponent->IsValidLowLevelFast()) 
	{
		ClosingAudioComponent->SetSound(ClosingSound);
	}

	if (ActorCurve)
	{
		////Add the float curve to the timeline and connect it to the interpfunction's delegate
		//MyTimeline->AddInterpFloat(DoorCurve, InterpFunction, FName("Alpha"));
		//// Add our OnTimelineFinished function
		//MyTimeline->SetTimelineFinishedFunc(TimelineFinished);

		//// Setting the timeline's settings before it starts
		//MyTimeline->SetLooping(false);
		//MyTimeline->SetIgnoreTimeDilation(true);

		//MyTimeline->SetComponentTick

		MyTimeline.AddInterpFloat(ActorCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		MyTimeline.SetLooping(false);
	}

}

void UInteractableDoor::SetOpen(bool bState)
{
	Open = bState;
}

// Called every frame
void UInteractableDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MyTimeline.TickTimeline(DeltaTime);

	/*if (PlayerPawn)
	{
		FVector PawnLocation = PlayerPawn->GetActorLocation();
		Direction = OwnerActor->GetActorLocation() - PawnLocation;
		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, OwnerActor->GetActorRotation());
	}*/

	/*if (GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, (TEXT("Yes it has the custcom component: " + Direction.ToString())));*/
}

void UInteractableDoor::PlaySoundOpening()
{
	if (ClosingAudioComponent->IsPlaying())
	{
		ClosingAudioComponent->SetPaused(true);
	}

	OpeningAudioComponent->SetPaused(false);
	OpeningAudioComponent->Play();
	if (!bMonsterUsage)
	{
	GameMode->InstantiateSoundWaveSphere(SoundLocation->GetComponentLocation(), 2000);
	}
}

void UInteractableDoor::PlaySoundClosing()
{
	if (OpeningAudioComponent->IsPlaying())
	{
		OpeningAudioComponent->SetPaused(true);
	}

	ClosingAudioComponent->SetPaused(false);
	ClosingAudioComponent->Play();
	if (!bMonsterUsage)
	{
		GameMode->InstantiateSoundWaveSphere(SoundLocation->GetComponentLocation(), 2000);
	}

}

void UInteractableDoor::PlaySoundSmashing()
{
	SmashingAudioComponent->Play();
}

void UInteractableDoor::StopAllSounds()
{
	if (OpeningAudioComponent->IsPlaying())
	{
		OpeningAudioComponent->Stop();
	}

	if (ClosingAudioComponent->IsPlaying())
	{
		ClosingAudioComponent->Stop();
	}
}

void UInteractableDoor::UpdateActorRotation()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = RotateValue * ActorCurve->GetFloatValue(TimelineValue);
	FQuat NewRotation;

	if (bBroken)
	{
		NewRotation = FQuat(FRotator(0.0f, CurveFloatValue, (-CurveFloatValue/30)));
	}
	else
	{
		NewRotation = FQuat(FRotator(0.0f, CurveFloatValue, 0.0f));
	}

	//Might have to change it if it doesn't work
	ActorToRotate->SetRelativeRotation(NewRotation);
}

void UInteractableDoor::Activation()
{
	// Will contain the code necessary to execute when another interactable object calls this one
}

void UInteractableDoor::ExecuteRotation()
{
	if (bReadyState)
	{
		//PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		/*FVector PawnLocation = PlayerPawn->GetActorLocation();
		FVector Direction = OwnerActor->GetActorLocation() - PawnLocation;
		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, OwnerActor->GetActorRotation());*/

		if (!Open)
		{
			if(OwnerActor->ActorHasTag("DoorType3"))
			{
				// Temporary fix for doors using the Y axis as the base instead of the X axis. Normally the smartest solution would be to modify one of the meshes.
				if (Direction.Y > 0.0f)
				{
					RotateValue = 1.0f;
				}
				else
				{
					RotateValue = -1.0f;
				}
			}
			else
			{
				if (Direction.X > 0.0f)
				{
					RotateValue = 1.0f;
				}
				else
				{
					RotateValue = -1.0f;
				}
			}

			bReadyState = false;

			MyTimeline.PlayFromStart();
			SetOpen(true);
			if (bBroken)
			{
				PlaySoundSmashing();
			}
			else
			{
				PlaySoundOpening();
			}
		}
		else
		{
			if (!bMonsterUsage)
			{
				bReadyState = false;
				MyTimeline.Reverse();
				SetOpen(false);
				PlaySoundClosing();
			}
		}
	}
	else
	{
		if (MyTimeline.IsReversing())
		{
			SetOpen(true);
			MyTimeline.Play();
			PlaySoundOpening();
		}
		else
		{
			if (!bMonsterUsage)
			{
				SetOpen(false);
				MyTimeline.Reverse();
				PlaySoundClosing();
			}
		}
	}
}

// Might need either another Interact with the number of keys required as a parameter either here or in the InteractableLock component

void UInteractableDoor::Interact()
{
	if (bActorInteraction)
	{
		if (!bBroken)
		{
			if (bUnlocked)
			{
				ExecuteRotation();
				SetMonsterUsage(false);
			}
			else
			{
				// Play a sound of locked door
			}
		}
	}
}

void UInteractableDoor::SetReadyState()
{
	bReadyState = true;
	StopAllSounds();

	TArray<UStaticMeshComponent*> OwnerStaticMeshComponents;
	OwnerActor->GetComponents<UStaticMeshComponent>(OwnerStaticMeshComponents);

	for (int i = 0; i < OwnerStaticMeshComponents.Num(); i++)
	{
		UStaticMeshComponent* TemporaryElement = OwnerStaticMeshComponents[i];

		// Check if it's possible to set the Door in editor instead of having to use that meh function
		if (TemporaryElement->GetFName() == "Door")
		{
			if (Open)
				TemporaryElement->SetCanEverAffectNavigation(true);
			else
				TemporaryElement->SetCanEverAffectNavigation(false);
		}
	}

}

void UInteractableDoor::SetDirection(AActor* InteractingActor)
{
	FVector PawnLocation = InteractingActor->GetActorLocation();
	Direction = OwnerActor->GetActorLocation() - PawnLocation;
	Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, OwnerActor->GetActorRotation());
}

void UInteractableDoor::BreakDoorOpen()
{
	if (!bBroken)
	{
		if (MyTimeline.IsPlaying())
		{
			bBroken = true;
			float tempTime = MyTimeline.GetPlaybackPosition();
			MyTimeline.Stop();
			MyTimeline.AddInterpFloat(BreakCurve, TimelineCallback);
			MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
			MyTimeline.SetLooping(false);
			MyTimeline.SetPlaybackPosition(tempTime, true);
			MyTimeline.Play();
		}
		else
		{
			if (!Open)
			{
				if (!MyTimeline.IsReversing())
				{
					bBroken = true;
					MyTimeline.AddInterpFloat(BreakCurve, TimelineCallback);
					MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
					MyTimeline.SetLooping(false);

					ExecuteRotation();
				}
			}
		}
	}	
}

