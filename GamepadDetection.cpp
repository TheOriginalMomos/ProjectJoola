

#include "GamepadDetection.h"
#include "SlateBasics.h"

UGamepadDetection::UGamepadDetection()
{
}

UGamepadDetection::~UGamepadDetection()
{
}

bool UGamepadDetection::IsGamepadConnected()
{
	auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	{
		return true;
	}
	return false;
}
