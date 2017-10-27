// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerState.h"
#include "MPlayerController.h"
#include "MCommand.h"
#include "GameFramework/GameMode.h"
#include "SimultaneousGameMode.generated.h"

/**
 * 
 */

UCLASS()
class MICE_API ASimultaneousGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:
	ASimultaneousGameMode();

	/** setup team changes at player login */
	void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
		void sumbitCommands(AMPlayerController* playerController);
	
protected:
	virtual void BeginPlay() override;

	ETeam ChooseTeam(APlayerState* playerState);

	/** select best spawn point for player */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void ExecuteCommands();
	
	TArray<UMCommand*> blueCommands;
	TArray<UMCommand*> redCommands;

	bool isBlueReady;
	bool isRedReady;
};
