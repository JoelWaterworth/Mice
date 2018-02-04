// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerState.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

class UMCommand;

class AMPlayerController;

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	US_Idle 		UMETA(DisplayName = "Idle"),
	US_Walk 		UMETA(DisplayName = "Walk"),
	US_Shooting		UMETA(DisplayName = "Shooting"),
	US_Overwatch	UMETA(DisplayName = "Overwatch"),
};

UCLASS(BlueprintType, Blueprintable)
class MICE_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnit();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* selectPlane;

	UPROPERTY(BlueprintReadWrite)
		TMap<FIntVector, FIntVector> roots;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Team")
		ETeam team;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* root;

public:

	UFUNCTION(server, reliable, WithValidation, BlueprintCallable)
		void UpdatePos(FIntVector n_pos);

	UPROPERTY(Replicated, BlueprintReadWrite)
		FIntVector pos;
	UPROPERTY(Replicated, BlueprintReadWrite)
		bool bHasExecutedCommand;
	UPROPERTY(Replicated, BlueprintReadWrite)
		UMCommand* command;
};
