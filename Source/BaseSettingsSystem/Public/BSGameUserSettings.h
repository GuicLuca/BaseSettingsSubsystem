// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023

#pragma once

#include "CoreMinimal.h"
#include "AudioMixerBlueprintLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "BSGameUserSettings.generated.h"

/**
 * @brief This delegate is fired when a problem append to let the user know what append.
 * If you call this delegate don't forget to use the macro NSLOCTEXT to track and localize the error message.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBSShowMessage, FText, message);

/**
 * @brief This delegate is a custom version of the OnAudioSwapCompleted to allow the blueprint know when the swap is completed.
 * @example You can plug the UI on this event to stop the waiting animation
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBSOnAudioDeviceSwapFinish, const FSwapAudioOutputResult&, SwapResult);

/**
 * This is the custom GameUserSetting object.
 * => Add property here to save new variables as settings values.
 * @example BSGameUserSettings.h
 * public :
 *      UFUNCTION(BlueprintCallable)
 *      void SetACustomSetting(FString InValue);
 *
 *      UFUNCTION(BlueprintPure)
 *      FString GetACustomSetting() const;
 *
 * protected:
 *      UPROPERTY(Config) // /!\ DO NOT FORGET TO MARK THE PROPERTY AS CONFIG /!\
 *      FString ACustomSetting;
 */
UCLASS()
class BASESETTINGSSYSTEM_API UBSGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()
	/**
	 * @brief Overriding method of the default class to add the applying process of new custom settings.
	 * @param bCheckForCommandLineOverrides true if you want to check command line overrides. (Set it ti FALSE by default)
	 */
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	/**
	 * @brief Overriding method of the default class to add the applying process of new custom settings that are not Resolution settings.
	 */
	virtual void ApplyNonResolutionSettings() override;

	/**
	 * @brief Overriding method of the default class to add the checking process to know if settings are dirty or not.
	 */
	virtual bool IsDirty() const override;

	/**
	 * @brief Here is the applying process of new custom settings that are about sound.
	 * called in ApplySettings, ApplyNonResolutionSettings to
	 */
	UFUNCTION(BlueprintCallable)
	void ApplySoundSettings();

	/**
	 * @brief This method is called to transform settings value that need to be set in the engine or wherever.
	 */
	void InitializeSettings(UGameInstance* InGameInstance);

	/**
	 * @brief Check if any updated settings need a restart to be effective.
	 * Add your own process here if you need for new custom settings.
	 * @return True if an updated setting need the game to restart to be applied.
	 */
	UFUNCTION(BlueprintPure, Category="BSSettings")
	bool IsRestartNeededToBeEffective() const;

	/** ==============
	 *     ACCESSORS
	 * ===============*/
	UFUNCTION(BlueprintCallable, Category="BSSettings")
	static UBSGameUserSettings* GetUBSGameUserSettings();

	UFUNCTION(BlueprintCallable, Category="BSSettings")
	void SetMasterVolume(float InVolume);
	UFUNCTION(BlueprintPure, Category="BSSettings")
	float GetMasterVolume() const;

	UFUNCTION(BlueprintCallable, Category="BSSettings")
	void SetMusicVolume(float InVolume);
	UFUNCTION(BlueprintPure, Category="BSSettings", Category="BSSettings")
	float GetMusicVolume() const;

	UFUNCTION(BlueprintCallable, Category="BSSettings")
	void SetFXVolume(float InVolume);
	UFUNCTION(BlueprintPure, Category="BSSettings")
	float GetFXVolume() const;

	UFUNCTION(BlueprintCallable, Category="BSSettings")
	void SetVoiceVolume(float InVolume);
	UFUNCTION(BlueprintPure)
	float GetVoiceVolume() const;

	UFUNCTION(BlueprintCallable, Category="BSSettings")
	void SetUIVolume(float InVolume);
	UFUNCTION(BlueprintPure, Category="BSSettings")
	float GetUIVolume() const;

	UFUNCTION(BlueprintCallable, Category="BSSettings")
	void SetAudioOutputDeviceId(FString InAudioID);
	UFUNCTION(BlueprintPure, Category="BSSettings")
	FString GetAudioOutputDeviceId() const;

	UFUNCTION(BlueprintPure, Category="BSSettings")
	float GetLastFPSLimitValueSaved() const;

protected:
	/**
	 * @brief Method called by ApplySettings methods (ApplyNonResolutionSettings, ApplySettings) to set the last FPS limit value saved.
	 * @param InFPSRate The new limit.
	 */
	void SetLastFPSLimitValueSaved(float InFPSRate);

	/**
	 * @brief Checking process to know if sound settings are dirty or not.
	 * @return True if a settings is updated and not saved
	 */
	bool IsSoundSettingsDirty() const;

	/**
	 * @brief Checking process to know if FPS settings are dirty or not.
	 * @return True if a settings is updated and not saved
	 */
	bool IsFPSSettingDirty() const;


	/**
	 * @brief Method called when the swap has completed. Use this method to check for swap errors or to display messages.
	 * @param SwapResult Status of the swap audio output command.
	 */
	UFUNCTION()
	void OnSwapCompletedHandler(const FSwapAudioOutputResult& SwapResult);

	// Reference to the game instance used to access special UObject that need to be referenced in Blueprint.
	UPROPERTY()
	TObjectPtr<UGameInstance> GameInstance;

	/** =================
	 *     Delegates
	 * ==================*/
	// This delegate is only used by the cpp to call OnSwapCompletedHandler method.
	UPROPERTY()
	FOnCompletedDeviceSwap OnSwapCompleted;
	// Use this delegate to let the Blueprint know when the swap is fully completed. (e.g. : Used for ui updated)
	UPROPERTY(BlueprintAssignable)
	FBSOnAudioDeviceSwapFinish OnSwapCompletedCustom;

	// This delegate allow you to display special message in popup for example.
	UPROPERTY(BlueprintAssignable)
	FBSShowMessage OnShowMessage;


	/** =============================
	 *     Custom settings values
	 * ==============================*/
	// Sound settings
	UPROPERTY(Config)
	float MasterVolume;
	UPROPERTY(Config)
	float MusicVolume;
	UPROPERTY(Config)
	float FXVolume;
	UPROPERTY(Config)
	float VoiceVolume;
	UPROPERTY(Config)
	float UIVolume;
	UPROPERTY(Config)
	FString AudioOutputDeviceId;

	/** ============================================================
	 *                        Controls variables
	 *   Variables used to controls if a value is updated or not.
	 * ============================================================*/
	// Represent the last fps limit saved
	UPROPERTY(Config)
	float LastFPSLimitValueSaved;
	// Represent the audio quality found when the game has start.
	float AudioQualityValueAtLoading;
	// Represent the last audio quality applied
	float LastAudioQualitySaved;
};
