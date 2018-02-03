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

	UFUNCTION(reliable, server, WithValidation, BlueprintCallable)
		void RunCommand(UMCommand* command, AUnit* unit);

	UPROPERTY(Replicated, BlueprintReadWrite)
		TMap<AUnit*, UMCommand*> commands;

	UPROPERTY(Replicated, BlueprintReadWrite)
		TMap<AUnit*, UMCommand*> CommandMap;

	UFUNCTION(Reliable, Client)
		void ClientPlayerUpdate(ETeam team);

	UFUNCTION(BlueprintNativeEvent)
		void UpdatePlayerTeam(ETeam team);
protected:

	virtual void BeginPlay() override;

public:
	// Called every frame

	UFUNCTION(BlueprintCallable)
		TMap<AUnit*, UMCommand*> GetCommands() { return commands; }

	UFUNCTION(BlueprintCallable)
		void ClearCommands() { commands.Empty(); }
};
