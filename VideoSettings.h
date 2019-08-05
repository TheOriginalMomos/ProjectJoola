
// Class containing blueprint accessible functions to change the game settings through UI elements.

#pragma once

#include "Public/RHI.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VideoSettings.generated.h"

#define MIN_SCREEN_WIDTH 1024
#define MIN_SCREEN_HEIGHT 768

/**
 *
 */
UCLASS()
class PROJECTJOOLA_API UVideoSettings : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool GetSupportedScreenResolutions(TArray<FString> &Resolutions);

	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool GetWindowTypes(TArray<TEnumAsByte<EWindowMode::Type>> &WindowTypes);

	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static EWindowMode::Type GetWindowType();

	// Get currently set screen resolution
	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static FString GetScreenResolution();

	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool IsInFullscreen();

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetScreenResolution(const int32 Width, const int32 Height, const EWindowMode::Type WindowType);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool ChangeScreenResolution(const int32 Width, const int32 Height, const EWindowMode::Type WindowType);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetVideoQualitySettings(int32 &AntiAliasing, int32 &Effects, int32 &PostProcess, int32 &Resolution,
			int32 &Shadow, int32 &Texture, int32 &ViewDistance);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetVideoQualitySettings(const int32 AntiAliasing = 3, const int32 Effects = 3, const int32 PostProcess = 3,
			const int32 Resolution = 100, const int32 Shadow = 3, const int32 Texture = 3, const int32 ViewDistance = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetAntiAliasingQualitySetting(int32 &AntiAliasing);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetAntiAliasingQualitySetting(const int32 AntiAliasing = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetEffectsQualitySetting(int32 &Effects);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetEffectsQualitySetting(const int32 Effects = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetPostProcessQualitySetting(int32 &PostProcess);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetPostProcessQualitySetting(const int32 PostProcess = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetResolutionQualitySetting(int32 &Resolution);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetResolutionQualitySetting(const int32 Resolution = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetShadowQualitySetting(int32 &Shadow);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetShadowQualitySetting(const int32 Shadow = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetTextureQualitySetting(int32 &Texture);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetTextureQualitySetting(const int32 Texture = 3);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool GetViewDistanceQualitySetting(int32 &ViewDistance);

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetViewDistanceQualitySetting(const int32 ViewDistance = 3);

	// Check whether or not we have vertical sync enabled.
	UFUNCTION(BlueprintPure, Category = "Video Settings")
		static bool IsVSyncEnabled();

	// Set the vertical sync flag.
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SetVSyncEnabled(const bool VSync);

	// Confirm and save current video mode (resolution and fullscreen/windowed) as well as quality settings.
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool SaveVideoModeAndQuality();

	// Revert to original video settings.
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
		static bool RevertVideoMode();

private:

	static UGameUserSettings* GetGameUserSettings();

};
