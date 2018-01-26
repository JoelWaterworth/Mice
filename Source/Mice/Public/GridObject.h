// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GridObject.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	D_Forward 	UMETA(DisplayName = "Forward"),
	D_Backward 	UMETA(DisplayName = "Backward"),
	D_Rightward UMETA(DisplayName = "Rightward"),
	D_Leftward 	UMETA(DisplayName = "Leftward"),
	D_Upward	UMETA(DisplayName = "Upward"),
	D_Downward 	UMETA(DisplayName = "Downward"),
};

USTRUCT(BlueprintType)
struct FGridTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntVector Origin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDirection Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FIntVector> WalkablePosistions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FIntVector> BlockedTiles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int blockPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRightHandCorner; //this is for the corners of walls where the mesh will prevent objects passing on two different directions. Furthermore the second direction will always be clockwise to the direction of the grid object

public:

	FGridTransform(
		FIntVector origin = FIntVector(0),
		TArray<FIntVector> walkablePosistions = TArray<FIntVector>(),
		EDirection direction = EDirection::D_Forward,
		TArray<FIntVector> blockedTiles = TArray<FIntVector>(),
		int bp = 100,
		bool border = false,
		bool corner = false) :
			Origin(origin),
			WalkablePosistions(walkablePosistions),
			Direction(direction),
			BlockedTiles(blockedTiles),
			blockPercentage(bp),
			isBorder(border),
			isRightHandCorner(corner){}
};


USTRUCT(BlueprintType)
struct FGridObjectTree
{
	GENERATED_BODY()

		FGridTransform Trans;
		TArray<FGridObjectTree> Children;
public:

	FGridObjectTree(
		FGridTransform trans = FGridTransform(),
		TArray<FGridObjectTree> children = TArray<FGridObjectTree>()) :
		Trans(trans),
		Children(children) {}
};

UCLASS()
class MICE_API AGridObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridObject();

protected:

	virtual void PostEditMove(bool bFinished) override;
	
public:

	//A grid object can be made up of multiple objects because of this it has many posistions.
	//After every move of the actor this array is emptied and repopulated with new posistions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGridTransform GridOrigin;
	
	//if it has no children this will be empty
	UFUNCTION(BlueprintCallable)
		FGridObjectTree GetObjectTree();

	void UpdatePosition();
};
