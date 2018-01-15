// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCollision.h"
#include "Unit.h"
#include "GridObject.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WorldGrid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWorld, Log, All);

USTRUCT(BlueprintType)
struct FObstucle {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EDirection> direction;//because mutlple walls can occupy the same wall tile this must be an array.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isboarder;

public:

	FObstucle(
		bool b = false,
		TArray<EDirection> d = TArray<EDirection>()) :
		direction(d),
		isboarder(b) {}
};

USTRUCT(BlueprintType)
struct FGridTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWalkable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UGridCollision* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* DebugTextRender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AUnit* Unit;

public:

	FGridTile(
		bool w = false,
		UGridCollision* col = nullptr,
		UTextRenderComponent* text = nullptr,
		AUnit* u = nullptr) : 
			isWalkable(w),
			CollisionBox(col),
			DebugTextRender(text),
			Unit(u) {}
};

UCLASS(BlueprintType, Blueprintable)
class MICE_API AWorldGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInstancedStaticMeshComponent* InstanceMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AUnit> spawnClass;

	UPROPERTY()
		USceneComponent* root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		int32 height;

	UPROPERTY(EditAnywhere, Category = "Init")
		bool Refresh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		int32 width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		int32 spacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		FVector CollisionExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foo")
		TMap<FIntVector, FGridTile> gridTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Garbage")
		TArray<USceneComponent*> waste;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
		TArray<FIntVector> BlueSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
		TArray<FIntVector> RedSpawnPoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void DebugPath(TMap<FIntVector, float> gScore);

	TArray<FIntVector> GetNeighbours(FIntVector origin);

	UPROPERTY()
		TMap<FIntVector, FObstucle> obstucles;

	UPROPERTY()
		TArray<AGridObject*> GridObjects;

private:
	static void SetDeltaNStep(int32& delta, int32& step);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool PlotLine(FVector start, FVector end);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The Transform is of the centre of the tile
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTransform VectorToWorldTransform(FIntVector pos);

	//The Transform is of the centre of the tile
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTransform VectorToLocalTransform(FIntVector pos);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FIntVector LocationToVector(FVector currentPos);

	UFUNCTION(BlueprintCallable)
		TMap<FIntVector, FIntVector> CalculatePaths(AUnit* Unit, int32 Limit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static TArray<FIntVector> CreatePathFromRoutes(TMap<FIntVector, FIntVector> cameFrom, FIntVector Dest);
};