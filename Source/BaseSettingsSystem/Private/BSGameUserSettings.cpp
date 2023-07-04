// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023


#include "BSGameUserSettings.h"

#include "AudioDevice.h"
#include "BSettingsSubsystem.h"
#include "BSGameInstance.h"


/**
 * @example BSGameUserSettings.cpp
 *
 * void UBSGameUserSettings::SetACustomSetting(Fstring InValue){
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
	MasterVolume = 1;
	MusicVolume = 1;
	VoiceVolume = 1;
	FXVolume = 1;
	UIVolume = 1;
	LastFPSLimitValueSaved = GetFrameRateLimit();
	AudioQualityValueAtLoading = GetAudioQualityLevel();
	LastAudioQualitySaved = AudioQualityValueAtLoading;

	OnSwapCompleted.BindUFunction(this, "OnSwapCompletedHandler");
}

void UBSGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	// Call ApplySoundSettings first because ApplySettings call SaveSettings
	ApplySoundSettings();

	// Set the new last fps value saved
	SetLastFPSLimitValueSaved(GetFrameRateLimit());
	Super::ApplySettings(bCheckForCommandLineOverrides);
}

void UBSGameUserSettings::ApplyNonResolutionSettings()
{
	ApplySoundSettings();
	// Set the new last fps value saved
	SetLastFPSLimitValueSaved(GetFrameRateLimit());
	Super::ApplyNonResolutionSettings();
}

void UBSGameUserSettings::ApplySoundSettings()
{
	// Set sound class from Game instance
	if (GameInstance)
	{
		const UBSGameInstance* GI = CastChecked<UBSGameInstance>(GameInstance);
		GI->SC_Master->Properties.Volume = FMath::Log2(MasterVolume);
		GI->SC_Music->Properties.Volume = FMath::Log2(MusicVolume);
		GI->SC_Voice->Properties.Volume = FMath::Log2(VoiceVolume);
		GI->SC_FX->Properties.Volume = FMath::Log2(FXVolume);
		GI->SC_UI->Properties.Volume = FMath::Log2(UIVolume);
	}
	
	FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice();
	if (AudioDevice)
	{
		FAudioQualitySettings AudioSettings = AudioDevice->GetQualityLevelSettings();
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
		const UBSGameInstance* GI = CastChecked<UBSGameInstance>(GameInstance);
		if (GI->SC_Master->Properties.Volume != FMath::Log2(MasterVolume)) bIsDirty = true;
		if (GI->SC_Music->Properties.Volume != FMath::Log2(MusicVolume)) bIsDirty = true;
		if (GI->SC_Voice->Properties.Volume != FMath::Log2(VoiceVolume)) bIsDirty = true;
		if (GI->SC_FX->Properties.Volume != FMath::Log2(FXVolume)) bIsDirty = true;
		if (GI->SC_UI->Properties.Volume != FMath::Log2(UIVolume)) bIsDirty = true;
	}

	if (AudioQualityLevel != LastAudioQualitySaved) bIsDirty = true;

	return bIsDirty;
}

bool UBSGameUserSettings::IsFPSSettingDirty() const
{
	return LastFPSLimitValueSaved != GetFrameRateLimit();
}

bool UBSGameUserSettings::IsDirty() const
{
	UE_LOG(LogBaseSettingsSubsystem, Warning,
	       TEXT("Super::IsDirty() = %hhd, IsSoundSettingsDirty() = %hhd, IsFPSSettingDirty() = %hhd"), Super::IsDirty(),
	       IsSoundSettingsDirty(), IsFPSSettingDirty());
	return Super::IsDirty() || IsSoundSettingsDirty() || IsFPSSettingDirty();
}

bool UBSGameUserSettings::IsRestartNeededToBeEffective() const
{
	return (LastAudioQualitySaved != AudioQualityValueAtLoading);
}

void UBSGameUserSettings::InitializeSettings(UGameInstance* InGameInstance)
{
	GameInstance = InGameInstance;
	if (!AudioOutputDeviceId.IsEmpty())
	{
		UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GameInstance->GetWorld(),AudioOutputDeviceId, OnSwapCompleted);
	}
}

void UBSGameUserSettings::OnSwapCompletedHandler(const FSwapAudioOutputResult& SwapResult)
{
	if (SwapResult.Result != ESwapAudioOutputDeviceResultState::Success)
	{
		OnShowMessage.Broadcast(NSLOCTEXT("BaseSettingsSubsystem", "AudioSwapFailed", "La changement de sortie audio a échoué."));
	}
	
	OnSwapCompletedCustom.Broadcast(SwapResult);
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
	return  UIVolume;
}

void UBSGameUserSettings::SetAudioOutputDeviceId(FString InAudioID)
{
	AudioOutputDeviceId = InAudioID;
	if (!AudioOutputDeviceId.IsEmpty())
	{
		UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GameInstance->GetWorld(),AudioOutputDeviceId, OnSwapCompleted);
	}
}

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

