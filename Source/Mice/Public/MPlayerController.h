// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "MCommand.h"
#include "MPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MICE_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMPlayerController();
	int32 Limit;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> CursorClass;

	UFUNCTION(reliable, server, WithValidation, BlueprintCallable)
		void ServerSubmitCommands();

	UPROPERTY(Replicated, BlueprintReadWrite)
		TArray<UMCommand*> commands;

	UFUNCTION(Reliable, Client)
		void ClientPlayerUpdate(ETeam team);

	UFUNCTION(BlueprintNativeEvent)
		void UpdatePlayerTeam(ETeam team);
protected:

	AUnit* Selected;

	AActor* Cursor;

	virtual void BeginPlay() override;

public:
	// Called every frame

	UFUNCTION(BlueprintCallable)
	TArray<UMCommand*> GetCommands() const { return commands; }

	UFUNCTION(BlueprintCallable)
		void ClearCommands() { commands.Empty(); }
};
