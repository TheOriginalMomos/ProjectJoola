

#include "InteractableThrowable.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "TimerManager.h"

UInteractableThrowable::UInteractableThrowable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InteractionType = EInteractionType::Throwable;

	CollisionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Collision"));
	CollisionAudioComponent->bAutoActivate = false;

	//CollisionAudioComponent->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	//CollisionAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//CollisionAudioComponent->AttachTo(OwnerActor->GetRootComponent());
}

void UInteractableThrowable::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent = Cast<UStaticMeshComponent>(OwnerActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	if (CollisionAudioComponent->IsValidLowLevelFast())
	{
		CollisionAudioComponent->SetSound(CollisionSound);
	}


	if (StaticMeshComponent)
	{
		CollisionAudioComponent->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		CollisionAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		BoxCollision = Cast<UBoxComponent>(StaticMeshComponent->GetChildComponent(0));

		if (BoxCollision)
		{
			//BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &UInteractableThrowable::OnComponentBeginOverlap);
		}
		OwnerActor->OnActorHit.AddDynamic(this, &UInteractableThrowable::OnHit);
		//OwnerActor->OnActorBeginOverlap.AddDynamic(this, &UInteractableThrowable::OnActorBeginOverlap);
		//StaticMeshComponent->OnComponentHit.AddDynamic(this, &UInteractableThrowable::OnComponentHit);
		//StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &UInteractableThrowable::OnComponentBeginOverlap);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Did not find a staticMeshComponent "));
	}

}

void UInteractableThrowable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
}

void UInteractableThrowable::Interact()
{
}

void UInteractableThrowable::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector CurrentVelocity = SelfActor->GetVelocity();
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Velocity is " + FString::FromInt(CurrentVelocity.Size())));*/

	if (CurrentVelocity.Size() <= MinimumVelocityToReactOnHit)
	{

	}
	else
	{
		if (!bSoundSphereTimerRunning)
		{
			bSoundSphereTimerRunning = true;
			//OwnerActor->GetWorldTimerManager().SetTimer(SoundSphereTimerHandle, this, &UInteractableThrowable::GenerateSoundSphere, TimeBetweenSoundSpheres, false);
			GetWorld()->GetTimerManager().SetTimer(SoundSphereTimerHandle, this, &UInteractableThrowable::ResetTimer, TimeBetweenSoundEvents, false);
			GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), CurrentVelocity.Size());
			float NewVolume = UKismetMathLibrary::NormalizeToRange(CurrentVelocity.Size(), 0, 1000);
			CollisionAudioComponent->SetVolumeMultiplier(NewVolume);
			CollisionAudioComponent->Play();
		}
	}

	// Change it later to change the size of the sound sphere base on velocity on impatch of possible.
	//GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), 2000);
}

void UInteractableThrowable::OnComponentHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	//GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), 2000);
}

void UInteractableThrowable::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap "));
	if ((OtherActor != OwnerActor) && !(OtherActor->GetClass()->IsChildOf(ASoundSphere::StaticClass())))
	{
		GameMode->InstantiateSoundWaveSphere(OwnerActor->GetActorLocation(), 2000);
	}*/
}

void UInteractableThrowable::ResetTimer()
{
	bSoundSphereTimerRunning = false;
	GetWorld()->GetTimerManager().ClearTimer(SoundSphereTimerHandle);
}
