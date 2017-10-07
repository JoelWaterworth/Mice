// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridLib.h"
#include "Components/BoxComponent.h"
#include "GridCollision.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Collision, meta = (BlueprintSpawnableComponent))
class MICE_API UGridCollision : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector2 Struct")
		FVector2DInt pos;
};
