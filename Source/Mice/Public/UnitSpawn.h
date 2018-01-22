// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawn.generated.h"

UCLASS()
class MICE_API AUnitSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector origin;
protected:
	virtual void PostEditMove(bool bFinished) override;
};
