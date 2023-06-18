// Author : Lucas Guichard <lucasguichard127@gmail.com> - 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASESETTINGSSYSTEM_API UBSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_Master;
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_Music;
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_FX;
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_Voice;
	UPROPERTY(EditDefaultsOnly, Category="Settings|Audio")
	TObjectPtr<USoundClass> SC_UI;
};
