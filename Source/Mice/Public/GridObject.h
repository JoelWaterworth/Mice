// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridObject.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	D_Forward 	UMETA(DisplayName = "Forward"),
	D_Backward 	UMETA(DisplayName = "Backward"),
	D_Rightward UMETA(DisplayName = "Rightward"),
	D_Leftward 	UMETA(DisplayName = "Leftward")
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
		TArray<FIntVector> Posistions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWalkable;

public:

	FGridTransform(
		FIntVector origin = FIntVector(0),
		TArray<FIntVector> posistions = TArray<FIntVector>(),
		EDirection direction = EDirection::D_Forward,
		bool border = false,
		bool walkable = true) :
			Origin(origin),
			Posistions(posistions),
			Direction(direction),
			isBorder(border),
			isWalkable(walkable){}
};


struct FGridObjectTree
{

public:

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
