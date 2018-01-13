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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isBorder;

public:

	FGridTransform(
		FIntVector origin = FIntVector(0),
		TArray<FIntVector> walkablePosistions = TArray<FIntVector>(),
		EDirection direction = EDirection::D_Forward,
		TArray<FIntVector> blockedTiles = TArray<FIntVector>(),
		bool border = false) :
			Origin(origin),
			WalkablePosistions(walkablePosistions),
			Direction(direction),
			BlockedTiles(blockedTiles),
			isBorder(border){}
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
	FGridObjectTree GetObjectTree();
};
