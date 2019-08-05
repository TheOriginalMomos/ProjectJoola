

#include "EndScreenMenu.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

void UEndScreenMenu::LoadLevel(FString LevelName)
{
	// Works fine for now, but might have to change it to work with actual loading screens if we get there because it's a rather long time with no inputs before the level gets loaded.
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

bool UEndScreenMenu::IsButtonFocused(UButton *ButtonReceived)
{
	bool bResult;

	bResult = ButtonReceived->HasUserFocus(UGameplayStatics::GetPlayerController(this, 0));

	return bResult;
}

void UEndScreenMenu::setbGamepadUsing(bool result)
{
	bGamepadUsing = result;
}
