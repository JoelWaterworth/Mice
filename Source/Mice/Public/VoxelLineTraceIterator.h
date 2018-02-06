// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridObject.h"
#include "UObject/NoExportTypes.h"
#include "VoxelLineTraceIterator.generated.h"

USTRUCT(BlueprintType)
struct FVoxelResult
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntVector voxel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EDirection> direction;
public:
	FVoxelResult(
		FIntVector out = FIntVector(),
		TArray<EDirection> v = TArray<EDirection>()) :
		voxel(out),
		direction(v) {}
};

USTRUCT(BlueprintType)
struct FIteratorReturn
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector impact_point;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVoxelResult> in_voxels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVoxelResult out_voxel;

public:

	FIteratorReturn(
		FVector out = FVector(),
		TArray<FVoxelResult> v = TArray<FVoxelResult>(),
		FVoxelResult o_v = FVoxelResult()) :
			impact_point(out),
			in_voxels(v),
			out_voxel(o_v){}
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MICE_API UVoxelLineTraceIterator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	FVector start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	FVector direction;
	UFUNCTION(BlueprintCallable)
		FIteratorReturn next();
	
};
