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
	int32 Limit;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> CursorClass;

	UFUNCTION(reliable, server, WithValidation)
		void ServerSubmitCommands();

	UPROPERTY(Replicated)
		TArray<UMCommand*> commands;
protected:

	AUnit* Selected;

	AActor* Cursor;

	virtual void SetupInputComponent() override;

	void Target();

	void Select();

	virtual void BeginPlay() override;

	UFUNCTION(reliable, server, WithValidation)
		void AddCommand(AUnit* unit, const TArray<FVector2DInt> &path);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<UMCommand*> GetCommands();
};
