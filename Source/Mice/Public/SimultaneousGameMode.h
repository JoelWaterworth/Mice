// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SimultaneousGameMode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETeam : uint8
{
	T_Blue 	UMETA(DisplayName = "Blue"),
	T_Red 	UMETA(DisplayName = "Red")
};

UCLASS()
class MICE_API ASimultaneousGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:
	ASimultaneousGameMode();
	
protected:
	virtual void BeginPlay() override;
};
