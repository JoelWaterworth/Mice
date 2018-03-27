// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCollision.h"
#include "Unit.h"
#include "GridObject.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Misc/Crc.h"
#include "Engine/BlockingVolume.h"
#include "GameFramework/Actor.h"
#include "WorldGrid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWorld, Log, All);

USTRUCT(BlueprintType)
struct FObstucle {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int blockPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isUpToEdge;
public:

	FObstucle(
		int bp = 100,
		bool onEdge = true):
		blockPercentage(bp),
		isUpToEdge(onEdge){}
};

USTRUCT(BlueprintType)
struct FBoarderKey {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDirection Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntVector origin;

public:

	FBoarderKey(
		EDirection d = EDirection::D_Forward,
		FIntVector o = FIntVector()) :
		Direction(d),
		origin(o) {}

	bool operator==(const FBoarderKey& other) const
	{
		return Direction == other.Direction && origin == other.origin;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FBoarderKey& Key)
	{
		return FCrc::MemCrc_DEPRECATED(&Key, sizeof(FBoarderKey));
	}
};

USTRUCT(BlueprintType)
struct FGridTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWalkable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGridCollision* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* DebugTextRender;

public:

	FGridTile(
		bool w = false,
		UGridCollision* col = nullptr,
		UTextRenderComponent* text = nullptr) : 
			isWalkable(w),
			CollisionBox(col),
			DebugTextRender(text){}
};

UCLASS(BlueprintType, Blueprintable)
class MICE_API AWorldGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGrid();

	UPROPERTY(EditAnywhere, Category = "Init")
		bool Refresh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInstancedStaticMeshComponent* InstanceMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AUnit> spawnClass;

	UPROPERTY()
		USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
		int32 minX;
	UPROPERTY(VisibleAnywhere)
		int32 minY;

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

	UFUNCTION()
	bool checkBlockingVolume(FIntVector Pos, TArray<ABlockingVolume*> GridBlockingVolumes);

	void DebugPath(TMap<FIntVector, float> gScore);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FIntVector> GetNeighbours(FIntVector origin, bool bReturnObstucles = false);

	UPROPERTY(VisibleAnywhere)
		TMap<FIntVector, FObstucle> obstucles;

	UPROPERTY(VisibleAnywhere)
		TMap<FBoarderKey, FObstucle> WallObstucles;

	UPROPERTY()
		TArray<AGridObject*> GridObjects;

	void AddBlockingTiles(FGridTransform GridOrigin);

private:
	static void SetDeltaNStep(int32& delta, int32& step);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool PlotLine(FVector start, FVector end);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetSpawnPoints();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static TArray<EDirection> directionFromIntVector(FIntVector dir);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FIntVector directionToVector(EDirection dir);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static EDirection addDirection(EDirection a, EDirection b);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static EDirection oppersiteDirection(EDirection dir);

	//The Transform is of the centre of the tile
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTransform VectorToWorldTransform(FIntVector pos);
	//The Transform is of the centre of the tile
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float CalculateProbabilityOfShot(FVector start, FVector end, AUnit* unit);
	//The Transform is of the centre of the tile
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTransform VectorToLocalTransform(FIntVector pos);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FIntVector LocationToVector(FVector currentPos);

	UFUNCTION(BlueprintCallable)
		TMap<FIntVector, FIntVector> CalculatePaths(AUnit* Unit, int32 Limit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static TArray<FIntVector> CreatePathFromRoutes(TMap<FIntVector, FIntVector> cameFrom, FIntVector Dest);

	UFUNCTION(BlueprintCallable)
		TMap<FIntVector, FObstucle> GetObstucles() const { return obstucles; }

	UFUNCTION(BlueprintCallable)
		TMap<FBoarderKey, FObstucle> GetWallObstucles() const { return WallObstucles; }
};