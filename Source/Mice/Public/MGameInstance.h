// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class MICE_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UMGameInstance();
	
public:
	UPROPERTY(config)
		float Spacing;
};
