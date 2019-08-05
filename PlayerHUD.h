

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerCharacterHUD.h"
#include "EndScreenMenu.h"
#include "InteractableBase.h"
#include "InteractableDoor.h"
#include "InteractableDoorLocked.h"
#include "InteractableLever.h"
#include "GamepadDetection.h"
#include "PlayerHUD.generated.h"

/**
 * Should be used to draw the different menus and UI elements we will need.
 */
UCLASS()
class PROJECTJOOLA_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		FString BtnEndScreenMenuName = "BtnReturnMainMenu"; // Stores the name of the button you want to focus in EndScreenMenu

	UPROPERTY()
		FString BtnDeathScreenMenuName = "BtnReturnMainMenu"; // Stores the name of the button you want to focus in DeathScreenMenu

	UPROPERTY()
		FString BtnPauseScreenMenuName = "BtnReturnMainMenu";

	UPROPERTY()
		FString BtnSettingsScreenMenuName = "BtnReturnMainMenu";

	UPROPERTY()
		FString BtnHowToScreenMenuName = "BtnReturnMainMenu";

	UPROPERTY()
		FString BtnMainMenuName = "BtnPlayGame";

	UPROPERTY()
		FString BtnDesiredName; // Stores the name of the desired button to focus on

	UPROPERTY()
		UGamepadDetection* GamepadDetection;

	UPROPERTY()
		APlayerController* PlayerController;

	UPROPERTY()
		FString	InteractionTextDefault = "Press E to ";

	UPROPERTY()
		FString	InteractionText;

	UPROPERTY()
		FLinearColor ColorCrosshairCurrent;

	UPROPERTY()
		FLinearColor ColorCrosshairNext;

	UPROPERTY()
		FLinearColor ColorInteractable = FLinearColor(1.f, .0f, .0f, 1.f);

	UPROPERTY()
		FLinearColor ColorUninteractable = FLinearColor(1.0f, 1.0f, 1.0f, 1.f);

	UPROPERTY()
		FLinearColor ColorNoInteractable = FLinearColor(.1f, .1f, .1f, .25f);

	UPROPERTY()
		FString InteractionTypeText;

	UPROPERTY()
		UInteractableDoor* TemporaryDoor;

	UPROPERTY()
		UInteractableDoorLocked* TemporaryDoorLocked;

	UPROPERTY()
		UInteractableLever* TemporaryLever;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsPauseMenuDisplayed = false;

public:
	APlayerHUD();

	virtual void BeginPlay() override;

	void ShowPlayerCharacterHUD();

	// Displays the possibles basic interactions on the HUD
	void ShowPossibleInteraction(EInteractionType&);

	// Displays the possibles interactions on the HUD
	void ShowPossibleInteraction(EInteractionType&, EObjectType&, AActor*&);

	//Adds the EndScreenMenu widget on screen
	UFUNCTION(BlueprintCallable)
		void ShowEndScreenMenu();

	//Adds the DeatScreenMenu widget on screen
	UFUNCTION(BlueprintCallable)
		void ShowDeathScreenMenu();

	UFUNCTION(BlueprintCallable)
		void ShowPauseScreenMenu();

	UFUNCTION(BlueprintCallable)
		void ShowMainScreenMenu();

	UFUNCTION(BlueprintCallable)
		void ShowLoadingScreen();

	UFUNCTION(BlueprintCallable)
		void ShowSettingsScreen();

	UFUNCTION(BlueprintCallable)
		void ShowHowToScreen();

	// Setups the Menu controls depending if the player uses a controller or not
	UFUNCTION()
		void SetupMenuControls(UEndScreenMenu* CurrentMenuWidget, bool ShouldPauseGame = true);

	UFUNCTION()
		void DisableMenuControls();

	UFUNCTION(BlueprintCallable)
		void RemoveLoadingScreen();

	UFUNCTION(BlueprintCallable)
		void ClearScreenWidgets();

	UPROPERTY()
		UEndScreenMenu* CurrentMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wPlayerCharacterHUD; // Stores the widget used for the PlayerCharacterHUD
	UPROPERTY()
		UPlayerCharacterHUD* PlayerCharacterHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wPauseScreenMenu; // Stores the widget used for the End Screen Menu
	UPROPERTY()
		UEndScreenMenu* PauseScreenMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wEndScreenMenu; // Stores the widget used for the End Screen Menu
	UPROPERTY()
		UEndScreenMenu* EndScreenMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wDeathScreenMenu; // Stores the widget used for the Death Screen Menu
	UPROPERTY()
		UEndScreenMenu* DeathScreenMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wMainScreenMenu; // Stores the widget used for the Death Screen Menu
	UPROPERTY()
		UEndScreenMenu* MainScreenMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wLoadingScreen; // Stores the widget used for the Death Screen Menu
	UPROPERTY()
		UEndScreenMenu* LoadingScreenWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wSettingsMenu; // Stores the widget used for the Death Screen Menu
	UPROPERTY()
		UEndScreenMenu* SettingsMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wHowToMenu; // Stores the widget used for the Death Screen Menu
	UPROPERTY()
		UEndScreenMenu* HowToMenuWidget;

	UPROPERTY()
		EInteractionType CurrentInteractionType;

	/** Primary draw call for the HUD */
	//virtual void DrawHUD() override;


private:

};
