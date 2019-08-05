

#include "EndGameGoal.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"


// Sets default values
AEndGameGoal::AEndGameGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	EndZoneTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndZoneTrigger"));

	EndZoneTrigger->SetupAttachment(RootComponent);
	EndZoneTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EndZoneTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	EndZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEndGameGoal::OnOverlapBeginEndZoneTrigger);

	EndAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("End Music"));
	EndAudioComponent->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AEndGameGoal::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

	if (EndAudioComponent->IsValidLowLevelFast())
	{
		EndAudioComponent->SetSound(EndMusic);
	}
	
}

// Called every frame
void AEndGameGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndGameGoal::OnOverlapBeginEndZoneTrigger(UPrimitiveComponent* OverlappedComponent,AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetName() == "BP_PJCharacter_C_0")
	{
		UObject* WorldContextObject = GetWorld();

		UGameplayStatics::PlaySound2D(WorldContextObject, EndMusic, 0.25f);

		PlayerHUD->ShowEndScreenMenu();

	}
	

}

