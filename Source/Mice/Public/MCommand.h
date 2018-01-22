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
UCLASS(BlueprintType, Blueprintable)
class MICE_API UMCommand : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Run(AUnit* target);
};
