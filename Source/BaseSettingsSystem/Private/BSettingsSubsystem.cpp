// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023


#include "BSettingsSubsystem.h"
#include "BaseSettingsSystem.h"
#include "BSGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"


// Called when the game start to load settings and set values
void UBSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// Load settings from the .ini file :
	auto const GameUserSettings = UBSGameUserSettings::GetUBSGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogBaseSettingsSubsystem, Error,
			   TEXT("UBSettingsSubsystem::Initialize() : GameUserSettings class has not been set to BSGameUserSettings. Please set it in the project settings !"));
		return;
	}
	
	// Ensure settings value are valid before use the object.
	GameUserSettings->ValidateSettings();
	// Give a reference of the GameInstance to the GameUserSettings object.
	// (It will be used to get the SoundClass objects)
	GameUserSettings->InitializeSettings(GetGameInstance());
	
	// Apply loaded settings to the game
	GameUserSettings->ApplySettings(false);

	UE_LOG(LogBaseSettingsSubsystem, Display, TEXT("GameUserSettings loaded and applayed !"));
}

// Called before the game quit to save settings values
void UBSettingsSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// save settings :
	auto const GameUserSettings = UBSGameUserSettings::GetUBSGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogBaseSettingsSubsystem, Error,
			   TEXT("UBSettingsSubsystem::Deinitialize() : GameUserSettings class has not been set to BSGameUserSettings. Please set it in the project settings !"));
		return;
	}
	
	GameUserSettings->SaveSettings();
	UE_LOG(LogBaseSettingsSubsystem, Display, TEXT("GameUserSettings saved !"));
}

