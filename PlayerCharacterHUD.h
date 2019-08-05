

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Image.h"
#include "PlayerCharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJOOLA_API UPlayerCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetMyImage();

	UPROPERTY(BlueprintReadOnly)
		FText CurrentInteractionText;
	
	UPROPERTY(BlueprintReadOnly)
		FSlateBrush CrosshairSlateBrush;


};
