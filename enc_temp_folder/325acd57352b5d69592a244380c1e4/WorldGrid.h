// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCollision.h"
#include "Unit.h"
#include "GridLib.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WorldGrid.generated.h"

USTRUCT(BlueprintType)
struct FGridTile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isWalkable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UGridCollision* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* DebugTextRender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AUnit* Unit;

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

UCLASS(ClassGroup = Collision, meta = (BlueprintSpawnableComponent))
class MICE_API AWorldGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGrid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInstancedStaticMeshComponent* InstanceMesh;

	UPROPERTY()
		USceneComponent* root;

	UPROPERTY()
		TMap<FVector2DInt, FGridTile> GridTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		int32 height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		int32 width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
		int32 spacing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<USceneComponent*> waste;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform & Transform);
	
};
