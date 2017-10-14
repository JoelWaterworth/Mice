// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "MPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class MICE_API AMPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Team")
		ETeam team;
};
