// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerController.h"
#include "MCommand.h"
#include "GameFramework/GameState.h"
#include "MGameState.generated.h"

/**
 * 
 */
UCLASS()
class MICE_API AMGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMGameState();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SumbitCommands(AMPlayerController* playerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SumbitCommand(AMPlayerController* playerController, AUnit* unit);

	UFUNCTION(BlueprintCallable)
		void ExecuteCommand(UMCommand* command, AUnit* target);

protected:
	UFUNCTION(BlueprintCallable)
		void ExecuteCommands(TMap<AUnit*, UMCommand*> commands);

	UPROPERTY(Replicated)
		TMap<AUnit*, UMCommand*> blueCommands;
	UPROPERTY(Replicated)
		TMap<AUnit*, UMCommand*> redCommands;

	UPROPERTY(EditAnywhere, Replicated)
	bool isBlueReady;
	UPROPERTY(EditAnywhere, Replicated)
	bool isRedReady;
};