
// PLayer pawn used for the main menu scene. Can't move. Only used for camera transitions and such.

#include "PJMainMenuPawn.h"
#include "CameraLocation.h"

// Sets default values
APJMainMenuPawn::APJMainMenuPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APJMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineCallback;
	FOnTimelineEventStatic OnTimelineFinishedCallback;

	// Initialize timeline component for and assign callback functions
	if (CurveFloat != NULL)
	{
		TimelineComponent = NewObject<UTimelineComponent>(this, FName("TimelineAnim"));
		TimelineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		this->BlueprintCreatedComponents.Add(TimelineComponent);
		TimelineComponent->SetPropertySetObject(this);
		TimelineComponent->SetDirectionPropertyName(FName("TimelineDirection"));
		TimelineComponent->SetLooping(false);
		//TimelineComponent->SetTimelineLength(InterpFloat);
		TimelineComponent->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		TimelineComponent->SetPlaybackPosition(0.0f, false);

		OnTimelineCallback.BindUFunction(this, FName(TEXT("TimelineCallback")));
		OnTimelineFinishedCallback.BindUFunction(this, FName(TEXT("TimelineFinishedCallback")));
		TimelineComponent->AddInterpFloat(CurveFloat, OnTimelineCallback, FName(TEXT("NAME_None")), FName(TEXT("MainInterp")));
		TimelineComponent->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
		TimelineComponent->RegisterComponent();
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraLocationsContainer::StaticClass(), Actors);
	ActorContainer = Cast<ACameraLocationsContainer>(Actors[0]);

	GameMode = Cast<AProjectJoolaGameModeMainMenu>(GetWorld()->GetAuthGameMode());
	PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	MoveCameraInstantly(ActorContainer->ListofCameras[0]->GetActorLocation(),
		ActorContainer->ListofCameras[0]->GetActorRotation()
	);

#if UE_BUILD_SHIPPING
	//PlayerHUD->ShowLoadingScreen();
	GetWorldTimerManager().SetTimer(StartupDelayTimerHandle, this, &APJMainMenuPawn::ExecuteIntroTransition, StartupDelay, false);
	//UGameplayStatics::SetGamePaused(this, true);
#endif

#if !UE_BUILD_SHIPPING
	ExecuteIntroTransition();
#endif
}

// Called every frame
void APJMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// This pawn is for the main menu so no inputs are required.
void APJMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("MoveSide", this, &APJMainMenuPawn::ChangeCameraIndex);

}

void APJMainMenuPawn::MoveCameraInstantly(const FVector newLoc, const FRotator newRot)
{
	SetActorLocationAndRotation(newLoc, newRot);
}

// Executes the camera transition for the game's intro sequence.
void APJMainMenuPawn::ExecuteIntroTransition()
{
	PlayerHUD->ClearScreenWidgets();
	CurCam = ActorContainer->ListofCameras[0];
	if (CurCam->NextCamera != nullptr)
	{
		TransitioningToMenu = true;
		NewCam = CurCam->NextCamera;
		TimelineComponent->PlayFromStart();
	}
}

// Executes the camera transition for the game when the play button is pressed.
void APJMainMenuPawn::ExecuteGameTransition()
{
	CurCam = ActorContainer->ListofCameras[1];
	PlayerHUD->ClearScreenWidgets();

	// Selects the initial camera for the transition
	// and transitions to it.
	if (ActorContainer->ListofCameras[2] != nullptr)
	{
		TransitioningToGame = true;
		NewCam = ActorContainer->ListofCameras[2];
		TimelineComponent->PlayFromStart();
	}
}

void APJMainMenuPawn::ChangeCameraIndex(float val)
{
	// If the index is valid, move instantly to the next camera location.
	if (val > 0.0f)
	{
		GameMode->NextCameraIndex();
		ACameraLocation* cam = GameMode->GetCameraActor();
		MoveCameraInstantly(cam->GetActorLocation(), cam->GetActorRotation());
	}
}

void APJMainMenuPawn::TimelineCallback(float val)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Orange, FString::SanitizeFloat(val), true);
	FVector loc = FMath::Lerp(CurCam->GetActorLocation(), NewCam->GetActorLocation(), val);
	FRotator rot = FMath::Lerp(CurCam->GetActorRotation(), NewCam->GetActorRotation(), val);
	SetActorLocationAndRotation(loc, rot);
}

// Camera transition timer callback
void APJMainMenuPawn::TimelineFinishedCallback()
{
	CurCam = NewCam;
	// If the current camera has a next camera pointer then transition to this camera.
	if (NewCam->NextCamera)
	{
		NewCam = CurCam->NextCamera;
		TimelineComponent->PlayFromStart();
	}
	// If the play button has been clicked, do the transition and then load the next scene.
	else if (TransitioningToGame && CurCam == ActorContainer->ListofCameras[2]->NextCamera)
	{
		TransitioningToGame = false;
		PlayerHUD->ShowLoadingScreen();
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		UGameplayStatics::OpenLevel(GetWorld(), "Master_Level1");
	}
	// If the transition to the main menu camera is complete, display the main menu UI.
	else if (TransitioningToMenu && CurCam == ActorContainer->ListofCameras[1])
	{
		TransitioningToMenu = false;
		PlayerHUD->ShowMainScreenMenu();
	}
}

// Main Menu UI buttons.
void APJMainMenuPawn::BtnPlayGame_Clicked()
{
	for (AActor* Interactable : ActorContainer->InteractablesToTrigger)
	{
		Interactable->FindComponentByClass<UInteractableBase>()->Interact();
	}
	ExecuteGameTransition();
}

void APJMainMenuPawn::BtnSettings_Clicked()
{
	PlayerHUD->ClearScreenWidgets();
	PlayerHUD->ShowSettingsScreen();
}

void APJMainMenuPawn::BtnHowTo_Clicked()
{
	PlayerHUD->ClearScreenWidgets();
	PlayerHUD->ShowHowToScreen();
}
