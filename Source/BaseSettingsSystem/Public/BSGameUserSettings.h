// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023

#pragma once

#include "CoreMinimal.h"
#include "AudioMixerBlueprintLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "BSGameUserSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBSShowMessage, FText, message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBSOnAudioDeviceSwapFinish,const FSwapAudioOutputResult&, SwapResult);

/**
 * This is the custom game user setting object. Add property here to save new variables has settings values.
 * @example BSGameUserSettings.h
 * public :
 *      UFUNCTION(BlueprintCallable)
 *      void SetACustomSetting(FString InValue);
 *
 *      UFUNCTION(BlueprintPure)
 *      FString GetACustomSetting() const;
 *
 * protected:
 *      UPROPERTY(Config)
 *      FString ACustomSetting;
 */
UCLASS()
class BASESETTINGSSYSTEM_API UBSGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	virtual void ApplyNonResolutionSettings() override;
	virtual bool IsDirty() const override;

	UFUNCTION(BlueprintCallable)
	void ApplySoundSettings();

	void InitializeSettings(UGameInstance* InGameInstance);

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

	UFUNCTION(BlueprintPure, Category="BSSettings")
	bool IsRestartNeededToBeEffective() const;

protected:
	void SetLastFPSLimitValueSaved(float InFPSRate);
	
	bool IsSoundSettingsDirty() const;
	bool IsFPSSettingDirty() const;

	UPROPERTY()
	FOnCompletedDeviceSwap OnSwapCompleted;
	UPROPERTY(BlueprintAssignable)
	FBSOnAudioDeviceSwapFinish OnSwapCompletedCustom;
	
	UFUNCTION()
	void OnSwapCompletedHandler(const FSwapAudioOutputResult& SwapResult);

	// This delegate allow you to display special message in popup for exemple.
	UPROPERTY(BlueprintAssignable)
	FBSShowMessage OnShowMessage;

	UPROPERTY()
	TObjectPtr<UGameInstance> GameInstance;

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

	// Controls variables
	UPROPERTY(Config)
	float LastFPSLimitValueSaved;
	// Represent the audio quality found when the game has start.
	float AudioQualityValueAtLoading;
	// Represent the last audio quality applied
	float LastAudioQualitySaved;
};
