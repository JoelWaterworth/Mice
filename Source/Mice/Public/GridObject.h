// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridObject.generated.h"

UCLASS()
class MICE_API AGridObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridObject();

protected:

	virtual void PostEditMove(bool bFinished) override;

	//these relative treate the origin of the object as zero. 
	TArray<FIntVector> RelativePosistions;
	
public:

	//A grid object can be made up of multiple objects because of this it has many posistions.
	//After every move of the actor this array is emptied and repopulated with new posistions.
	TArray<FIntVector> posistions;
	
	
};
