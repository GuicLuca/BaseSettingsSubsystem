// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BSGameInstance.generated.h"

/**
 * @brief This class need to be inherited by your own GameInstance class.
 * It contains simple reference to object that the Settings subsystem need but can't keep in memory itself.
 * This GameInstance class has no logic and must only be used to keep reference of object for the settings subsystem.
 */
UCLASS()
class BASESETTINGSSYSTEM_API UBSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Reference of the Master SoundClass UObject. It will be used
	 * by the settings subsystem to get and set the volume or the sound quality.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_Master;
	/**
	 * @brief Reference of the Music SoundClass UObject. It will be used
	 * by the settings subsystem to get and set the volume or the sound quality.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_Music;
	/**
	 * @brief Reference of the FX SoundClass UObject. It will be used
	 * by the settings subsystem to get and set the volume or the sound quality.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_FX;
	/**
	 * @brief Reference of the Voice SoundClass UObject. It will be used
	 * by the settings subsystem to get and set the volume or the sound quality.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_Voice;
	/**
	 * @brief Reference of the UI SoundClass UObject. It will be used
	 * by the settings subsystem to get and set the volume or the sound quality.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_UI;
};
