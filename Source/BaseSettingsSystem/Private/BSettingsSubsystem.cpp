// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023


#include "BSettingsSubsystem.h"
#include "BSGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"

DEFINE_LOG_CATEGORY(LogBaseSettingsSubsystem);

// Called when the game start to load settings and set values
void UBSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// Load settings from the .ini file :
	auto const GameUserSettings = UBSGameUserSettings::GetUBSGameUserSettings();
	GameUserSettings->ValidateSettings(); // Ensure settings value are valid before use the object.
	GameUserSettings->InitializeSettings(GetGameInstance());
	// Apply loaded settings to set the game in the same configuration as the last session.
	GameUserSettings->ApplySettings(false);

	UE_LOG(LogBaseSettingsSubsystem, All, TEXT("GameUserSettings loaded and applayed !"));
}

// Called before the game quit to save settings values
void UBSettingsSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// save settings :
	auto const GameUserSettings = UBSGameUserSettings::GetUBSGameUserSettings();
	GameUserSettings->SaveSettings();
	UE_LOG(LogBaseSettingsSubsystem, All, TEXT("GameUserSettings saved !"));
}

