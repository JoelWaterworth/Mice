// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPlayerState.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	T_Blue 	UMETA(DisplayName = "Blue"),
	T_Red 	UMETA(DisplayName = "Red")
};


/**
 * 
 */
UCLASS()
class MICE_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMPlayerState();

	ETeam team;
};
