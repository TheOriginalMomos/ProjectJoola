

#include "PlayerHUD.h"
#include "PJCharacter.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

APlayerHUD::APlayerHUD()
{
	// The following lines can be removed technically if the PlayerHUD is turned into a BP since we can then set the variables in editor.
	static ConstructorHelpers::FClassFinder<UPlayerCharacterHUD> PlayerCharacterHUDObj(TEXT("/Game/Blueprints/UIs/BP_PlayerCharacterHUD"));
	wPlayerCharacterHUD = PlayerCharacterHUDObj.Class;

	static ConstructorHelpers::FClassFinder<UEndScreenMenu> EndScreenMenuObj(TEXT("/Game/Blueprints/UIs/BP_EndScreenMenu"));
	wEndScreenMenu = EndScreenMenuObj.Class;

	static ConstructorHelpers::FClassFinder<UWidget> DeathScreenMenuObj(TEXT("/Game/Blueprints/UIs/BP_DeathScreenMenu"));
	wDeathScreenMenu = DeathScreenMenuObj.Class;

	static ConstructorHelpers::FClassFinder<UWidget> PauseScreenMenuObj(TEXT("/Game/Blueprints/UIs/BP_PauseScreenMenu"));
	wPauseScreenMenu = PauseScreenMenuObj.Class;

	static ConstructorHelpers::FClassFinder<UWidget> MainScreenMenuObj(TEXT("/Game/Blueprints/UIs/BP_MainMenu"));
	wMainScreenMenu = MainScreenMenuObj.Class;

	static ConstructorHelpers::FClassFinder<UWidget> LoadingScreenObj(TEXT("/Game/Blueprints/UIs/BP_LoadingScreen"));
	wLoadingScreen = LoadingScreenObj.Class;

	static ConstructorHelpers::FClassFinder<UWidget> SettingsScreenObj(TEXT("/Game/Blueprints/UIs/BP_SettingsMenu"));
	wSettingsMenu = SettingsScreenObj.Class;

	static ConstructorHelpers::FClassFinder<UWidget> HowToScreenObj(TEXT("/Game/Blueprints/UIs/BP_Controls"));
	wHowToMenu = HowToScreenObj.Class;
}

void APlayerHUD::BeginPlay()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	//TimelineComponentCrosshair = Cast<APJCharacter>(GetOwner())->TimelineComponentCrosshairColor->PlayFromStart();
	//PlayerController->InputComponent->BindAction("PauseMenu", IE_Pressed, PlayerController, &APlayerHUD::ShowPauseScreenMenu);
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("PauseMenu", IE_Pressed, this, &APlayerHUD::ShowPauseScreenMenu).bExecuteWhenPaused = true;

	if (wPlayerCharacterHUD)
	{
		PlayerCharacterHUDWidget = CreateWidget<UPlayerCharacterHUD>(GetWorld(), wPlayerCharacterHUD);

		InteractionText = InteractionTextDefault;
	}

	if (wEndScreenMenu)
	{
		EndScreenMenuWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wEndScreenMenu);
	}

	if (wDeathScreenMenu)
	{
		DeathScreenMenuWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wDeathScreenMenu);
	}

	if (wPauseScreenMenu)
	{
		PauseScreenMenuWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wPauseScreenMenu);
	}

	if (wMainScreenMenu)
	{
		MainScreenMenuWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wMainScreenMenu);
	}

	if (wLoadingScreen)
	{
		LoadingScreenWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wLoadingScreen);
	}

	if (wSettingsMenu)
	{
		SettingsMenuWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wSettingsMenu);
	}

	if (wHowToMenu)
	{
		HowToMenuWidget = CreateWidget<UEndScreenMenu>(GetWorld(), wHowToMenu);
	}

	// Temporary call to show the playerHUD on screen. Will be called elsewhere once the menu screen 'N the loadings are in
	ShowPlayerCharacterHUD();
#if UE_BUILD_SHIPPING
	ShowLoadingScreen();
#endif
}

void APlayerHUD::ShowPlayerCharacterHUD()
{
	if (PlayerCharacterHUDWidget)
	{
		PlayerCharacterHUDWidget->AddToViewport();
	}
}

void APlayerHUD::ShowPossibleInteraction(EInteractionType& InteractionTypeReceived)
{
	switch (InteractionTypeReceived)
	{
	case EInteractionType::Unlockable:
		InteractionTypeText = "Unlock";
		// Possibly add a check to see if it's a door and if so, if it's unlocked or not
		// Then add another check to verify if the door is open or not
		ColorCrosshairNext = ColorUninteractable;
		UE_LOG(LogTemp, Warning, TEXT("We are in UnlockableType Here"));
		break;
	case EInteractionType::Grabbable:
		InteractionTypeText = "Grab";
		ColorCrosshairNext = ColorInteractable;
		UE_LOG(LogTemp, Warning, TEXT("We are in GrabbableType here"));
		break;

	case EInteractionType::Throwable:
		InteractionTypeText = "Grab";
		ColorCrosshairNext = ColorInteractable;
		UE_LOG(LogTemp, Warning, TEXT("We are in ThrowableType here"));
		// Might want to add some code and some UI elements to show the controls to throw or release object. Or not since it takes just a bit of experimentation to figure it out
		break;

	case EInteractionType::Default:
		InteractionTypeText = "Interact";
		ColorCrosshairNext = ColorInteractable;
		UE_LOG(LogTemp, Warning, TEXT("Default type"));
		break;
	case EInteractionType::NoType:
		InteractionTypeText = "";
		InteractionText = "";
		ColorCrosshairNext = ColorNoInteractable;
		UE_LOG(LogTemp, Warning, TEXT("No Type"));
		break;

	default:
		InteractionTypeText = "Error";
		break;
	}

	// Normally, you should not access variables that way, but I figured since it's the only variable in the class, it wasn't very useful to make a set function just for that.
	InteractionText += InteractionTypeText;
	PlayerCharacterHUDWidget->CurrentInteractionText = FText::FromString(InteractionText);
	PlayerCharacterHUDWidget->GetMyImage()->Brush.TintColor = FSlateColor(ColorCrosshairNext);
	InteractionText = InteractionTextDefault;
}

void APlayerHUD::ShowPossibleInteraction(EInteractionType &InteractionTypeReceived, EObjectType &ObjectTypeReceived, AActor *&ActorReceived)
{
	switch (InteractionTypeReceived)
	{
	case EInteractionType::Unlockable:
		switch (ObjectTypeReceived)
		{
		case EObjectType::Door:
			TemporaryDoorLocked = ActorReceived->FindComponentByClass<UInteractableDoorLocked>();
			switch (TemporaryDoorLocked->GetLockType())
			{
			case ELockType::LeverLock:
				InteractionTypeText = "";
				InteractionText = "";
				ColorCrosshairNext = ColorUninteractable;
				break;
			case ELockType::KeyLock:
				InteractionText = "unlock";
				ColorCrosshairNext = ColorUninteractable;
				break;
			}
			break;

		default:
			break;
		}
		break;

	case EInteractionType::Default:
		switch (ObjectTypeReceived)
		{
		case EObjectType::Door:
			TemporaryDoor = ActorReceived->FindComponentByClass<UInteractableDoor>();
			if (TemporaryDoor->GetBrokenState())
			{
				InteractionText = "The door is broken";
				InteractionTypeText = "";
				ColorCrosshairNext = ColorUninteractable;
			}
			else
			{
				ColorCrosshairNext = ColorInteractable;
				if (TemporaryDoor->GetOpenState())
				{
					InteractionTypeText = "close";
				}
				else
				{
					InteractionTypeText = "open";
				}
			}
			break;

		case EObjectType::Lever:
			TemporaryLever = ActorReceived->FindComponentByClass<UInteractableLever>();
			ColorCrosshairNext = ColorInteractable;
			if (TemporaryLever->GetbActivated())
			{
				InteractionTypeText = "deactivate";
			}
			else
			{
				InteractionTypeText = "activate";
			}
			break;
		default:
			break;
		}
		break;

	}


	// Normally, you should not access variables that way, but I figured since it's the only variable in the class, it wasn't very useful to make a set function just for that.
	InteractionText += InteractionTypeText;
	PlayerCharacterHUDWidget->CurrentInteractionText = FText::FromString(InteractionText);
	PlayerCharacterHUDWidget->GetMyImage()->Brush.TintColor = FSlateColor(ColorCrosshairNext);
	InteractionText = InteractionTextDefault;
}

void APlayerHUD::ShowEndScreenMenu()
{
	if (EndScreenMenuWidget)
	{
		BtnDesiredName = BtnEndScreenMenuName;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Current widget: " + EndScreenMenuWidget->GetName()));
		EndScreenMenuWidget->AddToViewport();
		SetupMenuControls(EndScreenMenuWidget);
	}
}

void APlayerHUD::ShowDeathScreenMenu()
{
	if (DeathScreenMenuWidget)
	{
		BtnDesiredName = BtnDeathScreenMenuName;
		DeathScreenMenuWidget->AddToViewport();
		SetupMenuControls(DeathScreenMenuWidget);
	}
}

void APlayerHUD::ShowPauseScreenMenu()
{
	if (PauseScreenMenuWidget)
	{
		if (!bIsPauseMenuDisplayed)
		{
			bIsPauseMenuDisplayed = true;
			BtnDesiredName = BtnPauseScreenMenuName;
			PauseScreenMenuWidget->AddToViewport();
			SetupMenuControls(PauseScreenMenuWidget);
		}
		else
		{
			bIsPauseMenuDisplayed = false;
			PauseScreenMenuWidget->RemoveFromViewport();
			DisableMenuControls();
		}
	}
}

void APlayerHUD::ShowMainScreenMenu()
{
	if (MainScreenMenuWidget)
	{
		BtnDesiredName = BtnMainMenuName;
		MainScreenMenuWidget->AddToViewport();
		SetupMenuControls(MainScreenMenuWidget, false);
	}
}

void APlayerHUD::ShowLoadingScreen()
{
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->AddToViewport();
	}
}

void APlayerHUD::ShowHowToScreen()
{
	if (HowToMenuWidget)
	{
		BtnDesiredName = BtnHowToScreenMenuName;
		HowToMenuWidget->AddToViewport();
		SetupMenuControls(MainScreenMenuWidget, false);
	}
}

void APlayerHUD::ShowSettingsScreen()
{
	if (SettingsMenuWidget)
	{
		BtnDesiredName = BtnSettingsScreenMenuName;
		SettingsMenuWidget->AddToViewport();
		SetupMenuControls(MainScreenMenuWidget, false);
	}
}


void APlayerHUD::SetupMenuControls(UEndScreenMenu* CurrentMenuWidget, bool ShouldPauseGame)
{
	if (ShouldPauseGame)
		UGameplayStatics::SetGamePaused(this, true);
	PlayerController->SetInputMode(FInputModeGameAndUI());

	if (GamepadDetection->IsGamepadConnected())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Current widget: " + CurrentMenuWidget->GetName()));
		CurrentMenuWidget->GetWidgetFromName(FName(*BtnDesiredName))->SetUserFocus(PlayerController);
	}
	else
	{
		PlayerController->bShowMouseCursor = true;
	}
}

void APlayerHUD::DisableMenuControls()
{
	UGameplayStatics::SetGamePaused(this, false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
}

void APlayerHUD::RemoveLoadingScreen()
{
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->RemoveFromViewport();
	}
}

void APlayerHUD::ClearScreenWidgets()
{
	if (PlayerCharacterHUDWidget)
	{
		PlayerCharacterHUDWidget->RemoveFromViewport();
		SettingsMenuWidget->RemoveFromViewport();
		MainScreenMenuWidget->RemoveFromViewport();
		HowToMenuWidget->RemoveFromViewport();
		EndScreenMenuWidget->RemoveFromViewport();
		DeathScreenMenuWidget->RemoveFromViewport();
		LoadingScreenWidget->RemoveFromViewport();
	}
}
