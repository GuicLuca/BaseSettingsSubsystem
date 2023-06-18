// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023

#pragma once

#include "CoreMinimal.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BSettingsSubsystem.generated.h"

class UBSGameUserSettings;
DECLARE_LOG_CATEGORY_EXTERN(LogBaseSettingsSubsystem, Log, Log);

/**
 * @brief THis class ensure the loading and saving setting process of your game.
 * By default they are load at the start of the game and saved before the game quit.
 * You can call Load and Save function from your blueprint when you want.
 *
 * @details Add protected variable to this class and public Getter/Setter to save it.
 * Once done, call the getter in SaveSettingsToSave and the setter in LoadSettingsFromSave.
 * You also need to add a variable to the SaveGame Class @see BSettingsSaveGame.h
 */
UCLASS()
class BASESETTINGSSYSTEM_API UBSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
