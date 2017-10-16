// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "MCommand.h"
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
protected:

	AUnit* Selected;

	AActor* Cursor;

	virtual void SetupInputComponent() override;

	//void Target();

	//void Select();

	virtual void BeginPlay() override;

public:
	// Called every frame

	TArray<UMCommand*> GetCommands();
};
