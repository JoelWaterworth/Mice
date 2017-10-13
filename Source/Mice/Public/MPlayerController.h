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
protected:

	AUnit* Selected;

	AActor* Cursor;

	TMap<FVector2DInt, FVector2DInt> roots;


	virtual void SetupInputComponent() override;

	void Target();

	void Select();

	TArray<FVector2DInt> RootsToPath(FVector2DInt dest, TMap<FVector2DInt, FVector2DInt> roots);

	virtual void BeginPlay() override;

	TArray<UMCommand*> commands;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<UMCommand*> GetCommands();
};
