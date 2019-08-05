// Character actor. Can run, crouch, walk, pick up objects, etc.

#include "PJCharacter.h"
#include "PlayerHUD.h"

// Sets default values
APJCharacter::APJCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	WalkSpeed = CharacterMovement->MaxWalkSpeed;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = PositionNormalCamera; // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Configures the audio components and attaches them to the player.

	AudioComponentWalking = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentWalking"));
	AudioComponentWalking->bAutoActivate = false;
	AudioComponentWalking->SetupAttachment(GetCapsuleComponent());

	AudioComponentRunning = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentRunning"));
	AudioComponentRunning->bAutoActivate = false;
	AudioComponentRunning->SetupAttachment(GetCapsuleComponent());

	AudioComponentCrouching = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentCrouching"));
	AudioComponentCrouching->bAutoActivate = false;
	AudioComponentCrouching->SetupAttachment(GetCapsuleComponent());

	AudioComponentFlashlightOn = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFlashlightOn"));
	AudioComponentFlashlightOn->bAutoActivate = false;
	AudioComponentFlashlightOn->SetupAttachment(GetCapsuleComponent());

	AudioComponentBreathing = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentBreathing"));
	AudioComponentBreathing->bAutoActivate = false;
	AudioComponentBreathing->SetupAttachment(GetCapsuleComponent());

	AudioComponentDying = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentDying"));
	AudioComponentDying->bAutoActivate = false;
	AudioComponentDying->SetupAttachment(GetCapsuleComponent());

	// Attaches the flashlight component.

	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComponent"));
	FlashlightComponent->SetupAttachment(FirstPersonCameraComponent);

	// Attaches the holding component to allow the player to grab objects.

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->RelativeLocation.Z = 0.0f;
	HoldingComponent->RelativeLocation.Y = 25.0f;
	HoldingComponent->SetupAttachment(FirstPersonCameraComponent);

}

// Called when the game starts or when spawned
void APJCharacter::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineCallback;
	FOnTimelineEventStatic OnTimelineFinishedCallback;

	CapsuleHalfHeightStanding = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	GameMode = (AProjectJoolaGameModeBase*)GetWorld()->GetAuthGameMode();
	PlayerCamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;


	CharacterMovement->MaxWalkSpeed = WalkSpeed;

	// Assigns sound effects to players.
	if (AudioComponentWalking->IsValidLowLevelFast())
	{
		AudioComponentWalking->SetSound(SEWalking);
	}
	if (AudioComponentCrouching->IsValidLowLevelFast())
	{
		AudioComponentCrouching->SetSound(SECrouching);
	}
	if (AudioComponentRunning->IsValidLowLevelFast())
	{
		AudioComponentRunning->SetSound(SERunning);
	}
	if (AudioComponentFlashlightOn->IsValidLowLevelFast())
	{
		AudioComponentFlashlightOn->SetSound(SEFlashlightOn);
	}
	if (AudioComponentBreathing->IsValidLowLevelFast())
	{
		AudioComponentBreathing->SetSound(SEBreathing);
	}
	if (AudioComponentDying->IsValidLowLevelFast())
	{
		AudioComponentDying->SetSound(SEDying);
	}


	// Initializes the movement sound effect timers.
	GetWorldTimerManager().SetTimer(CrouchedSoundTimerHandle, this, &APJCharacter::PlaySoundCrouching, CrouchSoundDelay, true);
	GetWorldTimerManager().SetTimer(WalkingSoundTimerHandle, this, &APJCharacter::PlaySoundWalking, WalkSoundDelay, true);
	GetWorldTimerManager().SetTimer(RunningSoundTimerHandle, this, &APJCharacter::PlaySoundRunning, RunSoundDelay, true);

	// Pauses the timers initally to avoid audio spam.
	GetWorldTimerManager().PauseTimer(CrouchedSoundTimerHandle);
	GetWorldTimerManager().PauseTimer(RunningSoundTimerHandle);
	GetWorldTimerManager().PauseTimer(WalkingSoundTimerHandle);

	CurrentSoundTimer = &WalkingSoundTimerHandle;

	// Set up the Timeline component that handles the crouching transition
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

		// Add the float curve to the timeline and connect it to interpolation
		OnTimelineCallback.BindUFunction(this, FName(TEXT("TimelineCallback")));
		OnTimelineFinishedCallback.BindUFunction(this, FName(TEXT("TimelineFinishedCallback")));
		TimelineComponent->AddInterpFloat(CurveFloat, OnTimelineCallback);
		TimelineComponent->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
		TimelineComponent->RegisterComponent();

	}
#if UE_BUILD_SHIPPING
	//UGameplayStatics::SetGamePaused(this, true);
	GetWorldTimerManager().SetTimer(StartupDelayTimerHandle, this, &APJCharacter::StartupDelayCompleted, StartupDelay, false);
#endif
}

// Called every frame
void APJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	GenerateMovementSounds();
	CalculateVisibility();
	GenerateLight();
}

// Called to bind functionality to input
void APJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APJCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveSide", this, &APJCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APJCharacter::StartRunning);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APJCharacter::EndRunning);
	PlayerInputComponent->BindAction("SprintToggle", IE_Pressed, this, &APJCharacter::ToggleRunning);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APJCharacter::ToggleCrouching);

	PlayerInputComponent->BindAction("Light", IE_Pressed, this, &APJCharacter::ToggleFlashLight);

	PlayerInputComponent->BindAction("CheatInvisible", IE_Pressed, this, &APJCharacter::PlayerInvisible);
	PlayerInputComponent->BindAction("CheatInvincible", IE_Pressed, this, &APJCharacter::PlayerInvincible);
	PlayerInputComponent->BindAction("CheatSound", IE_Pressed, this, &APJCharacter::PlayerMute);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APJCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APJCharacter::LookUpAtRate);
}

// Removes the loading screen from the HUD when level streaming is completed.
void APJCharacter::StartupDelayCompleted()
{
	//UGameplayStatics::SetGamePaused(this, false);
	Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->RemoveLoadingScreen();
}

void APJCharacter::MoveForward(float Val)
{
	bIsMovingAlongX = false;
	if (Val != 0.f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
		bIsMovingAlongX = true;

		if (bIsRunning)
		{
			if (Val <= 0.f)
			{
				EndRunning();
			}
			else
			{
				PlayerCamManager->PlayCameraShake(CameraShakeRunning, 1.f);
			}
		}
		else if (bIsCrouched)
		{
			PlayerCamManager->PlayCameraShake(CameraShakeCrouching, 1.f);
		}
		else
		{
			PlayerCamManager->PlayCameraShake(CameraShakeWalkingForward, 1.f);
		}
	}
}

void APJCharacter::MoveRight(float Val)
{
	bIsMovingAlongY = false;
	if (Val != 0.f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
		bIsMovingAlongY = true;
		if (bIsRunning && !bIsMovingAlongX)
		{
			EndRunning();
		}
	}
}

void APJCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APJCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APJCharacter::StartRunning()
{
	if (bCanRun)
	{
		if (bIsCrouched)
		{
			ToggleCrouching();
		}
		if (!bIsCrouched)
		{
			GetWorldTimerManager().SetTimer(StaminaTickTimerHandle, this, &APJCharacter::StaminaDecayTick, StaminaDecayTickRate, true);
			CharacterMovement->MaxWalkSpeed = RunSpeed;
			bIsRunning = true;
		}
	}
}

void APJCharacter::ToggleRunning()
{
	// Switches between running and walking.
	if (bIsRunning)
	{
		EndRunning();
	}
	else
	{
		StartRunning();
	}
}

void APJCharacter::EndRunning()
{
	// Stops running is player is currently running.
	if (bIsRunning)
	{
		GetWorldTimerManager().ClearTimer(StaminaTickTimerHandle);
		GetWorldTimerManager().SetTimer(StaminaRegenDelayTimerHandle, this, &APJCharacter::StaminaRegenDelayTick, StaminaRegenDelay, true);
		CharacterMovement->MaxWalkSpeed = WalkSpeed;
		bIsRunning = false;
	}
}

void APJCharacter::ToggleCrouching()
{
	// Returns the player to a standing position unless there's an overhead obstruction.
	if (bIsCrouched && !CheckForCeiling())
	{
		TimelineComponent->Reverse();
		CharacterMovement->MaxWalkSpeed = WalkSpeed;
		bIsCrouched = false;
	}
	else
	{
		// If there's no ceiling, enable transition from crouching to standing.
		if (bIsRunning)
		{
			EndRunning();
		}
		TimelineComponent->Play();
		CharacterMovement->MaxWalkSpeed = CrouchSpeed;
		bIsCrouched = true;
	}
}

// Toggles the flashlight and plays the SE.
void APJCharacter::ToggleFlashLight()
{
	PlaySoundFlashlightOn();
	FlashlightComponent->ToggleVisibility();
	bIsFlashlightOn = FlashlightComponent->bVisible;

}

void APJCharacter::GenerateMovementSounds()
{
	// If the player is moving then start the sound effect timer for the current movement type.
	if (IsMoving())
	{
		if (bIsCrouched)
		{
			// Pause the previous movement type SE timer and enable the crouched SE timer.
			// 
			if (*CurrentSoundTimer != CrouchedSoundTimerHandle)
				GetWorldTimerManager().PauseTimer(*CurrentSoundTimer);
			CurrentSoundTimer = &CrouchedSoundTimerHandle;
			GetWorldTimerManager().UnPauseTimer(*CurrentSoundTimer);
		}
		else if (bIsRunning)
		{
			// Pause the previous movement type SE timer and enable the running SE timer.
			if (*CurrentSoundTimer != RunningSoundTimerHandle)
				GetWorldTimerManager().PauseTimer(*CurrentSoundTimer);
			CurrentSoundTimer = &RunningSoundTimerHandle;
			GetWorldTimerManager().UnPauseTimer(*CurrentSoundTimer);
		}
		else
		{
			// Pause the previous movement type SE timer and enable the walking SE timer.
			if (*CurrentSoundTimer != WalkingSoundTimerHandle)
				GetWorldTimerManager().PauseTimer(*CurrentSoundTimer);
			CurrentSoundTimer = &WalkingSoundTimerHandle;
			GetWorldTimerManager().UnPauseTimer(*CurrentSoundTimer);
		}
	}
	else
	{
		// If the player is not moving, pause the current timer.
		GetWorldTimerManager().PauseTimer(*CurrentSoundTimer);
	}
}

void APJCharacter::StaminaDecayTick()
{
	// Everytime the stamina timer ticks, adjust the breathing SE to be louder the lower the stamina is.
	// When stamina reaches 0, disable sprinting.
	if (!AudioComponentBreathing->IsPlaying())
		AudioComponentBreathing->FadeIn(1.f, AudioComponentBreathing->VolumeMultiplier, 0.f);
	if (bIsMovingAlongX)
		if (StaminaValue > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Stamina: %d"), StaminaValue));
			StaminaValue -= StaminaDecayValue;
			AudioComponentBreathing->SetVolumeMultiplier((StaminaMaxValue - StaminaValue) * 0.02f);
		}
		else
		{
			StaminaValue = 0;
			bCanRun = false;
			EndRunning();
		}
}

void APJCharacter::StaminaRegenDelayTick()
{
	// Once the stamina regen delay timer ends, enable stamina regeneration.
	GetWorldTimerManager().ClearTimer(StaminaRegenDelayTimerHandle);
	GetWorldTimerManager().SetTimer(StaminaTickTimerHandle, this, &APJCharacter::StaminaRegenTick, StaminaRegenTickRate, true);
}

void APJCharacter::StaminaRegenTick()
{
	// Everytime this timer ticks, while stamina is not full, increment the stamina value and adjust the breathing SE.
	if (StaminaValue < StaminaMaxValue)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Stamina: %d"), StaminaValue));
		StaminaValue += StaminaRegenValue;
		if (!bCanRun)
			AudioComponentBreathing->SetVolumeMultiplier((StaminaMaxValue - StaminaValue) * 0.02f);
	}
	else
	{
		// When stamina is full, fade out the breathing SE and disable the stamina regen timer.
		StaminaValue = StaminaMaxValue;
		AudioComponentBreathing->FadeOut(4.f, 0.f);
		bCanRun = true;
		GetWorldTimerManager().ClearTimer(StaminaTickTimerHandle);
		//GetWorldTimerManager().SetTimer(StaminaRegenDelayTimerHandle, this, &APJCharacter::StaminaRegenDelayTick, StaminaRegenDelay, true);
	}
}

// Calculates the player's visibility stat depending on the state of the flashlight and the type of movement they're using.
// Affects the AI's ability to detect the player.
void APJCharacter::CalculateVisibility()
{
	if (bIsCrouched)
	{
		if (IsMoving())
		{
			Visibility = MovCrouchVision;
		}
		else
		{
			Visibility = IdleCrouchVision;
		}
	}
	else if (!bIsRunning)
	{
		if (IsMoving())
		{
			Visibility = MovVisionWalk;
		}
		else
		{
			Visibility = IdleVision;
		}
	}
	else
		Visibility = VisionRun;

	if (bIsFlashlightOn)
		Visibility += FLAddVision;

	if (Visibility >= MaxVision)
		Visibility = MaxVision;

	if (bIsInvisible)
		Visibility = 0;
}

bool APJCharacter::CheckForCeiling()
{
	FVector CapsuleLoc = GetCapsuleComponent()->GetComponentLocation();

	// Calculate the capsule collision half height difference between standing and crouching.
	float CurrentCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float HeightDiff = CapsuleHalfHeightStanding - CurrentCapsuleHalfHeight;

	// Vector indicating the starting location of the raycast minus a small offset to detect extremely close collisions.
	FVector Start = FVector(CapsuleLoc.X, CapsuleLoc.Y, CapsuleLoc.Z + CurrentCapsuleHalfHeight - 0.02f);

	// Vector indicating the ending location of the raycast plus a small offset to have the desired height.
	FVector End = FVector(CapsuleLoc.X, CapsuleLoc.Y, CapsuleLoc.Z + HeightDiff + CapsuleHalfHeightStanding + 0.02f);

	// Raycast parameters such as adding the player's capsule to the list of ignored collisions.
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredComponent(GetCapsuleComponent());

	DrawDebugCapsule(GetWorld(),
		GetCapsuleComponent()->GetComponentLocation(),
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		GetCapsuleComponent()->GetScaledCapsuleRadius(),
		FQuat::Identity, FColor::Blue, false, 5.0f, 0, 1.0f);

	// Casts 2 rays beyond the previously defined initial ray in a circular fashion (360°) to cover
	// most of the capsule's top surface to determine if standing up would cause the player to
	// overlap with foreign geometry.
	int theta = 0;
	while (theta <= 360)
	{
		// Reset the radius after the rays for this angle have been cast.
		float radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		// Casts one ray at full and half radius
		for (int i = 0; i < 2; i++) // 2 rays for each theta
		{
			// Calculate the (x,y) coordinates of the ray.
			float x = radius * FMath::Cos(FMath::DegreesToRadians(theta));
			float y = radius * FMath::Sin(FMath::DegreesToRadians(theta));

			// Cast the ray using the (x,y) coordinates as an offset to the origin ray. Check for collisions with static geometry.
			GetWorld()->LineTraceSingleByChannel(Hit, Start + FVector(x, y, 0), End + FVector(x, y, 0), ECC_WorldStatic, CollisionParams);
			DrawDebugLine(GetWorld(), Start + FVector(x, y, 0), End + FVector(x, y, 0), FColor::Green, false, 5.0f, 0, 1.0f);

			// If a collision is detected with an overhead collider, there is no need 
			// to cast more rays as a portion of the player's capsule WILL collide.
			if (Hit.bBlockingHit)
			{
				UE_LOG(LogTemp, Log, TEXT("Collided with %s."), *Hit.GetComponent()->GetName());
				UE_LOG(LogTemp, Log, TEXT(" %f units above the player."), Hit.Distance);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Ceiling too low to stand up", false);
				return true;
			}
			radius *= 0.5f; // Divide radius by two
		}
		theta += 45; // Increment angle by 45 degrees
	}
	return false;
}

void APJCharacter::PlaySoundRunning()
{
	GameMode->InstantiateSoundWaveSphere(this->GetActorLocation(), RunSoundwaveRadius);
	AudioComponentRunning->Play();
}

void APJCharacter::PlaySoundWalking()
{
	GameMode->InstantiateSoundWaveSphere(this->GetActorLocation(), WalkSoundwaveRadius);
	AudioComponentWalking->Play();
}

void APJCharacter::PlaySoundCrouching()
{
	GameMode->InstantiateSoundWaveSphere(this->GetActorLocation(), CrouchSoundwaveRadius);
	AudioComponentCrouching->Play();
}

void APJCharacter::PlaySoundFlashlightOn()
{
	//GameMode->InstantiateSoundWaveSphere(this->GetActorLocation(), 100);
	AudioComponentFlashlightOn->Play();
}

void APJCharacter::PlaySoundBreathing()
{
	//GameMode->InstantiateSoundWaveSphere(this->GetActorLocation(), 100);
	AudioComponentBreathing->Play();
}

void APJCharacter::PlaySoundDying()
{
	//AudioComponentDying->Play();

	UGameplayStatics::PlaySound2D(GetWorld(), SEDying, 1.0f);
}

// Check if the player is moving at all.
bool APJCharacter::IsMoving()
{
	if (!bIsMovingAlongY && !bIsMovingAlongX)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void APJCharacter::GenerateLight()
{

	if (bIsFlashlightOn)
	{
		if (cptLight == 15)
		{
			UWorld* const World = GetWorld();
			const FRotator SpawnRotation = GetControlRotation();
			const FVector SpawnLocation = GetActorLocation();

			FActorSpawnParameters ActorSpawnParams;

			World->SpawnActor<ALightVision>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			cptLight = 0;
		}
		else
		{
			cptLight++;
		}
	}

}

// Smooths out the camera transition between standing and crouching.
void APJCharacter::TimelineCallback(float val)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(CapsuleHalfHeightStanding, GetCharacterMovement()->CrouchedHalfHeight, val), false);
	FVector PosCam = FVector(FirstPersonCameraComponent->RelativeLocation);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(PosCam.X, PosCam.Y, FMath::Lerp(BaseEyeHeight, CrouchedEyeHeight, val)));
}

// Callback when the camera transition is completed.
void APJCharacter::TimelineFinishedCallback()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Standing height: %f Crouched height: %f "), CapsuleHalfHeightStanding, GetCharacterMovement()->CrouchedHalfHeight));
}

// Debugging feature that makes the player invisible to the AI's field of view.
void APJCharacter::PlayerInvisible()
{
	if (!bIsInvisible)
	{
		bIsInvisible = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Player Invisibility On!");
	}
	else
	{
		bIsInvisible = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Player Invisibility Off!");
	}

}

// Debugging feature that makes the player invulnerable to the AI's attacks.
void APJCharacter::PlayerInvincible()
{
	if (!bIsInvincible)
	{
		bIsInvincible = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Player Invincibility On!");
	}
	else
	{
		bIsInvincible = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Player Invincibility Off!");
	}

}

// Debugging feature that disables the audio spheres generated by the player's actions and interactions.
void APJCharacter::PlayerMute()
{
	if (!bIsMute)
	{
		bIsMute = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Player Mute On!");
	}
	else
	{
		bIsMute = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Player Mute Off!");
	}

}

