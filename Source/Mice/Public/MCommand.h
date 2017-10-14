// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldGrid.h"
#include "Unit.h"
#include "UObject/NoExportTypes.h"
#include "MCommand.generated.h"

/**
 * 
 */
UCLASS()
class MICE_API UMCommand : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Replicated)
	int32 unitID;

	TArray<FVector2DInt> path;

	void Run();
};
