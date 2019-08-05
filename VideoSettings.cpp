
// Class containing blueprint accessible functions to change the game settings through UI elements.

#include "VideoSettings.h"
#include "ProjectJoola.h"

UGameUserSettings* UVideoSettings::GetGameUserSettings()
{
	if (GEngine != nullptr)
	{
		return GEngine->GameUserSettings;
	}

	return nullptr;
}

// Returns an array of supported resolutions.
bool UVideoSettings::GetSupportedScreenResolutions(TArray<FString> &Resolutions)
{
	FScreenResolutionArray ResolutionsArray;

	if (RHIGetAvailableResolutions(ResolutionsArray, true))
	{
		for (const FScreenResolutionRHI &Resolution : ResolutionsArray)
		{
			if (Resolution.Width < MIN_SCREEN_WIDTH || Resolution.Height < MIN_SCREEN_HEIGHT)
				continue;

			FString Str = FString::FromInt(Resolution.Width) + "x" + FString::FromInt(Resolution.Height);
			Resolutions.AddUnique(Str);
		}
		return true;
	}
	return false;
}

// Returns an array of window types (Borderless, Windowed, etc.)
bool UVideoSettings::GetWindowTypes(TArray<TEnumAsByte<EWindowMode::Type>> &WindowTypes)
{
	if (EWindowMode::NumWindowModes == 0)
	{
		return false;
	}

	for (int i = 0; i < EWindowMode::NumWindowModes; ++i)
	{
		WindowTypes.Add(EWindowMode::ConvertIntToWindowMode(i));
	}
	return true;
}

// Returns the current window type.
EWindowMode::Type UVideoSettings::GetWindowType()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return EWindowMode::Fullscreen;
	}

	return Settings->GetFullscreenMode();
}

// Returns the current screen resolution.
FString UVideoSettings::GetScreenResolution()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return FString("");
	}

	FIntPoint Resolution = Settings->GetScreenResolution();
	return FString::FromInt(Resolution.X) + "x" + FString::FromInt(Resolution.Y);
}

// Returns a boolean on whether or not the game is in fullscreen mode.
bool UVideoSettings::IsInFullscreen()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	return Settings->GetFullscreenMode() == EWindowMode::Fullscreen;
}

// Confirm the screen resolution and the window type changes.
bool UVideoSettings::SetScreenResolution(const int32 Width, const int32 Height, const EWindowMode::Type WindowType)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->SetScreenResolution(FIntPoint(Width, Height));
	Settings->SetFullscreenMode(WindowType);
	return true;
}

// Requests a screen resolution change. Use SetScreenResolution to confirm.
bool UVideoSettings::ChangeScreenResolution(const int32 Width, const int32 Height, const EWindowMode::Type WindowType)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->RequestResolutionChange(Width, Height, WindowType, false);
	return true;
}

bool UVideoSettings::GetVideoQualitySettings(int32 & AntiAliasing, int32 & Effects, int32 & PostProcess, int32 & Resolution, int32 & Shadow, int32 & Texture, int32 & ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	AntiAliasing = Settings->ScalabilityQuality.AntiAliasingQuality;
	Effects = Settings->ScalabilityQuality.EffectsQuality;
	PostProcess = Settings->ScalabilityQuality.PostProcessQuality;
	Resolution = Settings->ScalabilityQuality.ResolutionQuality;
	Shadow = Settings->ScalabilityQuality.ShadowQuality;
	Texture = Settings->ScalabilityQuality.TextureQuality;
	ViewDistance = Settings->ScalabilityQuality.ViewDistanceQuality;

	return true;
}

bool UVideoSettings::SetVideoQualitySettings(const int32 AntiAliasing, const int32 Effects, const int32 PostProcess, const int32 Resolution, const int32 Shadow, const int32 Texture, const int32 ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.AntiAliasingQuality = AntiAliasing;
	Settings->ScalabilityQuality.EffectsQuality = Effects;
	Settings->ScalabilityQuality.PostProcessQuality = PostProcess;
	Settings->ScalabilityQuality.ResolutionQuality = Resolution;
	Settings->ScalabilityQuality.ShadowQuality = Shadow;
	Settings->ScalabilityQuality.TextureQuality = Texture;
	Settings->ScalabilityQuality.ViewDistanceQuality = ViewDistance;
	return true;
}

bool UVideoSettings::GetAntiAliasingQualitySetting(int32 &AntiAliasing)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	AntiAliasing = Settings->ScalabilityQuality.AntiAliasingQuality;
	return true;
}

bool UVideoSettings::SetAntiAliasingQualitySetting(const int32 AntiAliasing)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.AntiAliasingQuality = AntiAliasing;
	return true;
}

bool UVideoSettings::GetEffectsQualitySetting(int32 &Effects)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Effects = Settings->ScalabilityQuality.EffectsQuality;
	return true;
}

bool UVideoSettings::SetEffectsQualitySetting(const int32 Effects)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.EffectsQuality = Effects;
	return true;
}

bool UVideoSettings::GetPostProcessQualitySetting(int32 &PostProcess)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	PostProcess = Settings->ScalabilityQuality.PostProcessQuality;
	return true;
}

bool UVideoSettings::SetPostProcessQualitySetting(const int32 PostProcess)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.PostProcessQuality = PostProcess;
	return true;
}

bool UVideoSettings::GetResolutionQualitySetting(int32 &Resolution)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Resolution = Settings->ScalabilityQuality.ResolutionQuality;
	return true;
}

bool UVideoSettings::SetResolutionQualitySetting(const int32 Resolution)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.ResolutionQuality = Resolution;
	return true;
}

bool UVideoSettings::GetShadowQualitySetting(int32 &Shadow)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Shadow = Settings->ScalabilityQuality.ShadowQuality;
	return true;
}

bool UVideoSettings::SetShadowQualitySetting(const int32 Shadow)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.ShadowQuality = Shadow;
	return true;
}

bool UVideoSettings::GetTextureQualitySetting(int32 &Texture)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Texture = Settings->ScalabilityQuality.TextureQuality;
	return true;
}

bool UVideoSettings::SetTextureQualitySetting(const int32 Texture)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.TextureQuality = Texture;
	return true;
}

bool UVideoSettings::GetViewDistanceQualitySetting(int32 &ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	ViewDistance = Settings->ScalabilityQuality.ViewDistanceQuality;
	return true;
}

bool UVideoSettings::SetViewDistanceQualitySetting(const int32 ViewDistance)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ScalabilityQuality.ViewDistanceQuality = ViewDistance;
	return true;
}

bool UVideoSettings::IsVSyncEnabled()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	return Settings->IsVSyncEnabled();
}

bool UVideoSettings::SetVSyncEnabled(const bool VSync)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->SetVSyncEnabled(VSync);
	return true;
}

bool UVideoSettings::SaveVideoModeAndQuality()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->ConfirmVideoMode();
	Settings->ApplyNonResolutionSettings();
	Settings->SaveSettings();
	return true;
}

bool UVideoSettings::RevertVideoMode()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return false;
	}

	Settings->RevertVideoMode();
	return true;
}
