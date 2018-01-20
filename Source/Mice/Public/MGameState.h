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
	
protected:
	UFUNCTION(BlueprintCallable)
		void ExecuteCommands(TArray<UMCommand*> commands);

	UPROPERTY(Replicated)
	TArray<UMCommand*> blueCommands;
	UPROPERTY(Replicated)
	TArray<UMCommand*> redCommands;

	UPROPERTY(EditAnywhere, Replicated)
	bool isBlueReady;
	UPROPERTY(EditAnywhere, Replicated)
	bool isRedReady;
};