// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCollision.h"
#include "Unit.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WorldGrid.generated.h"

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

	void DebugPath(TMap<FIntVector, int32> gScore);

	TArray<FIntVector> GetNeighbours(FIntVector origin);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTransform VectorToWorldTransform(FIntVector pos);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTransform VectorToLocalTransform(FIntVector pos);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FIntVector LocationToVector(FVector currentPos);

	UFUNCTION(BlueprintCallable)
		TMap<FIntVector, FIntVector> CalculatePaths(AUnit* Unit, int32 Limit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static TArray<FIntVector> CreatePathFromRoutes(TMap<FIntVector, FIntVector> cameFrom, FIntVector Dest);
};
