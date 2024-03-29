﻿// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BSettingsSubsystem.generated.h"

class UBSGameUserSettings;

/**
 * @brief This class ensure the loading and saving settings process of your game.
 * By default they are load at the start of the game and saved before the game quit.
 * You can call Load and Save function from your blueprint whenever you want.
 */
UCLASS()
class BASESETTINGSSYSTEM_API UBSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
