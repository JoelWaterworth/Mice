// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerState.h"
#include "MPlayerController.h"
#include "MCommand.h"
#include "GameFramework/GameMode.h"
#include "MGameMode.generated.h"

/**
 * 
 */

UCLASS()
class MICE_API AMGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:
	AMGameMode();

	/** setup team changes at player login */
	void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

	ETeam ChooseTeam(APlayerState* playerState);

	/** select best spawn point for player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
