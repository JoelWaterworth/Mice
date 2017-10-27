// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridLib.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

class AMPlayerController;

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	US_Idle 	UMETA(DisplayName = "Idle"),
	US_Walk 	UMETA(DisplayName = "Walk")
};

UCLASS(BlueprintType, Blueprintable)
class MICE_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnit();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* selectPlane;

	UPROPERTY(BlueprintReadWrite)
	TMap<FVector2DInt, FVector2DInt> roots;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* root;

public:

	UPROPERTY(BlueprintReadWrite)
		FVector2DInt pos;
};
