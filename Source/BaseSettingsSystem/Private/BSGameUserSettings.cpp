// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023


#include "BSGameUserSettings.h"

#include "AudioDevice.h"
#include "BaseSettingsSystem.h"
#include "BSGameInstance.h"


/**
 * @example BSGameUserSettings.cpp
 *
 * void UBSGameUserSettings::SetACustomSetting(Fstring& InValue){
 *      ACustomSetting = InValue;
 * }
 *
 * FString UBSGameUserSettings::GetACustomSetting() const {
 *		return ACustomSetting;
 * }
 */

UBSGameUserSettings::UBSGameUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set the default value of ACustomSetting here :
	// ACustomSetting = TEXT("I'm a setting added by the developer");
	MouseSensitivity = 1.f;
	
	MasterVolume = 2;
	MusicVolume = 2;
	VoiceVolume = 2;
	FXVolume = 2;
	UIVolume = 2;
	LastFPSLimitValueSaved = GetFrameRateLimit();
	AudioQualityValueAtLoading = GetAudioQualityLevel();
	LastAudioQualitySaved = AudioQualityValueAtLoading;

	CppOnSwapCompleted.BindUFunction(this, "OnSwapCompletedHandler");
}

void UBSGameUserSettings::InitializeSettings(UGameInstance* InGameInstance)
{
	if (!InGameInstance)
	{
		UE_LOG(LogBaseSettingsSubsystem, Error,
			   TEXT("UBSGameUserSettings::InitializeSettings() : InGameInstance is nullptr !"));
	}else if (auto const GI = Cast<UBSGameInstance>(InGameInstance))
	{
		GameInstance = GI;
	}
	else
	{
		UE_LOG(LogBaseSettingsSubsystem, Error,
			   TEXT("UBSGameUserSettings::InitializeSettings() : InGameInstance is not a UBSGameInstance !"));
		return;
	}
	
	if (!AudioOutputDeviceId.IsEmpty())
	{
		UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GameInstance->GetWorld(), AudioOutputDeviceId,
														   CppOnSwapCompleted);
	}
}

void UBSGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	// Call ApplySoundSettings first because ApplySettings call SaveSettings
	ApplySoundSettings();

	Super::ApplySettings(bCheckForCommandLineOverrides);

	// Set the new last fps value saved
	SetLastFPSLimitValueSaved(GetFrameRateLimit());
}

void UBSGameUserSettings::ApplyNonResolutionSettings()
{
	ApplySoundSettings();

	Super::ApplyNonResolutionSettings();

	// Set the new last fps value saved
	SetLastFPSLimitValueSaved(GetFrameRateLimit());
}

void UBSGameUserSettings::ApplySoundSettings()
{
	// Set sound class from Game instance
	if (GameInstance)
	{
		// Here we set the volume of the sound class with log2 to have a linear volume slider in the settings menu.
		if (GameInstance->SC_Master)
		{
			GameInstance->SC_Master->Properties.Volume = FMath::Log2(MasterVolume);
		}
		if (GameInstance->SC_Music)
		{
			GameInstance->SC_Music->Properties.Volume = FMath::Log2(MusicVolume);
		}
		if (GameInstance->SC_Voice)
		{
			GameInstance->SC_Voice->Properties.Volume = FMath::Log2(VoiceVolume);
		}
		if (GameInstance->SC_FX)
		{
			GameInstance->SC_FX->Properties.Volume = FMath::Log2(FXVolume);
		}
		if (GameInstance->SC_UI)
		{
			GameInstance->SC_UI->Properties.Volume = FMath::Log2(UIVolume);
		}
	}

	if (FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice())
	{
		const FAudioQualitySettings AudioSettings = AudioDevice->GetQualityLevelSettings();
		AudioDevice->SetMaxChannels(AudioSettings.MaxChannels);
	}
	LastAudioQualitySaved = GetAudioQualityLevel();
}

bool UBSGameUserSettings::IsSoundSettingsDirty() const
{
	bool bIsDirty = false;
	// Set sound class from Game instance
	if (GameInstance)
	{
		// Check if the volume of the sound class is different from the volume of the settings
		if (GameInstance->SC_Master->Properties.Volume != FMath::Log2(MasterVolume)) bIsDirty = true;
		if (GameInstance->SC_Music->Properties.Volume != FMath::Log2(MusicVolume)) bIsDirty = true;
		if (GameInstance->SC_Voice->Properties.Volume != FMath::Log2(VoiceVolume)) bIsDirty = true;
		if (GameInstance->SC_FX->Properties.Volume != FMath::Log2(FXVolume)) bIsDirty = true;
		if (GameInstance->SC_UI->Properties.Volume != FMath::Log2(UIVolume)) bIsDirty = true;
	}

	// Check if the audio quality level is different from the audio quality level of the settings
	if (AudioQualityLevel != LastAudioQualitySaved) bIsDirty = true;

	return bIsDirty;
}

bool UBSGameUserSettings::IsFPSSettingDirty() const
{
	return LastFPSLimitValueSaved != GetFrameRateLimit();
}

bool UBSGameUserSettings::IsDirty() const
{
	UE_LOG(LogBaseSettingsSubsystem, Log,
	       TEXT("Super::IsDirty() = %hhd, IsSoundSettingsDirty() = %hhd, IsFPSSettingDirty() = %hhd"), Super::IsDirty(),
	       IsSoundSettingsDirty(), IsFPSSettingDirty());
	
	return Super::IsDirty() || IsSoundSettingsDirty() || IsFPSSettingDirty();
}

bool UBSGameUserSettings::IsRestartNeededToBeEffective() const
{
	return (LastAudioQualitySaved != AudioQualityValueAtLoading);
}

void UBSGameUserSettings::ValidateSettings()
{
	Super::ValidateSettings();

	if (MouseSensitivity < 0) MouseSensitivity = 1.f;
	if (MasterVolume < 0) MasterVolume = 2;
	if (MusicVolume < 0) MusicVolume = 2;
	if (VoiceVolume < 0) VoiceVolume = 2;
	if (FXVolume < 0) FXVolume = 2;
	if (UIVolume < 0) UIVolume = 2;
	if (LastFPSLimitValueSaved < 0) LastFPSLimitValueSaved = GetFrameRateLimit();
}

void UBSGameUserSettings::OnSwapCompletedHandler(const FSwapAudioOutputResult& SwapResult) 
{
	if (SwapResult.Result != ESwapAudioOutputDeviceResultState::Success)
	{
		UE_LOG(LogBaseSettingsSubsystem, Warning,
		       TEXT("UBSGameUserSettings::OnSwapCompletedHandler() : SwapAudioOutputDevice failed !"));
	}

	OnSwapCompleted.Broadcast(SwapResult);
}

void UBSGameUserSettings::SetAudioOutputDeviceId(FString InAudioID)
{
	AudioOutputDeviceId = InAudioID;
	if (!AudioOutputDeviceId.IsEmpty() && GameInstance)
	{
		UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GameInstance->GetWorld(), AudioOutputDeviceId,
		                                                   CppOnSwapCompleted);
	}
}


// =============================
//   Getters and Setters
// =============================

FString UBSGameUserSettings::GetAudioOutputDeviceId() const
{
	return AudioOutputDeviceId;
}

float UBSGameUserSettings::GetLastFPSLimitValueSaved() const
{
	return LastFPSLimitValueSaved;
}

void UBSGameUserSettings::SetLastFPSLimitValueSaved(float InFPSRate)
{
	LastFPSLimitValueSaved = InFPSRate;
}

UBSGameUserSettings* UBSGameUserSettings::GetUBSGameUserSettings()
{
	return CastChecked<UBSGameUserSettings>(GetGameUserSettings());
}

void UBSGameUserSettings::SetMasterVolume(float InVolume)
{
	MasterVolume = InVolume;
}

float UBSGameUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

void UBSGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
}

float UBSGameUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UBSGameUserSettings::SetFXVolume(float InVolume)
{
	FXVolume = InVolume;
}

float UBSGameUserSettings::GetFXVolume() const
{
	return FXVolume;
}

void UBSGameUserSettings::SetVoiceVolume(float InVolume)
{
	VoiceVolume = InVolume;
}

float UBSGameUserSettings::GetVoiceVolume() const
{
	return VoiceVolume;
}

void UBSGameUserSettings::SetUIVolume(float InVolume)
{
	UIVolume = InVolume;
}

float UBSGameUserSettings::GetUIVolume() const
{
	return UIVolume;
}

void UBSGameUserSettings::SetMouseSensitivity(float InSensitivity)
{
	MouseSensitivity = InSensitivity;
}

float UBSGameUserSettings::GetMouseSensitivity() const
{
	return MouseSensitivity;
}
