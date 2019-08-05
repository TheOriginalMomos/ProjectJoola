

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TextWidgetTypes.h"
#include "EndScreenMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJOOLA_API UEndScreenMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	bool bGamepadUsing;

public:

	UFUNCTION(BlueprintCallable, Category = "Loading functions")
		void LoadLevel(FString LevelName); // Handles the loading of levels for the widgets. Might be changed later once we have multiple levels

	UFUNCTION(BlueprintCallable, Category = "Button functions")
		bool IsButtonFocused(UButton* ButtonReceived);


	UFUNCTION()
		void setbGamepadUsing(bool result);

	UFUNCTION()
		bool GetbGamepadUsing() { return bGamepadUsing; };

};
