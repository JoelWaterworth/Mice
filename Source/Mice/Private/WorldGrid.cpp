// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGrid.h"
#include "Algo/Reverse.h"
#include "../Public/WorldGrid.h"


// Sets default values
AWorldGrid::AWorldGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InstanceMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceMesh"));
	InstanceMesh->SetupAttachment(root);

	width = 10;
	height = 10;
	spacing = 120;

	CollisionExtent = FVector(55.0f, 55.0f, 10.0f);
}

// Called when the game starts or when spawned
void AWorldGrid::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AWorldGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform AWorldGrid::VectorToWorldTransform(FVector2DInt pos)
{

	return VectorToLocalTransform(pos) * GetActorTransform();
}

FTransform AWorldGrid::VectorToLocalTransform(FVector2DInt pos)
{
	FVector2DInt nPos = pos * spacing;
	return FTransform(FVector((float)nPos.x, (float)nPos.y, 0.0f));
}

TMap<FVector2DInt, FVector2DInt> AWorldGrid::CalculatePaths(AUnit * Unit, int32 Limit)
{
	TArray<FVector2DInt> OpenSet = TArray<FVector2DInt>();
	OpenSet.Add(Unit->pos);

	TArray<FVector2DInt> CloseSet = TArray<FVector2DInt>();

	TMap<FVector2DInt, int32> gScore = TMap<FVector2DInt, int32>();
	gScore.Add(Unit->pos, 0);
	TMap<FVector2DInt, FVector2DInt> roots = TMap<FVector2DInt, FVector2DInt>();
	while (OpenSet.Num() > 0) {
		FVector2DInt current = OpenSet[0];
		OpenSet.RemoveAt(0);
		CloseSet.Add(current);

		TArray<FVector2DInt> neighbours = GetNeighbours(current);

		for (FVector2DInt& neighbour : neighbours)
		{
			if (!CloseSet.Contains(neighbour)) {
				if (!OpenSet.Contains(neighbour)) {
					OpenSet.Add(neighbour);
				}
				int32* y = gScore.Find(current);
				if (y) {
					int32 currentScore = *y + 1;

					int32* x = gScore.Find(neighbour);

					if (x)
					{
						if ((currentScore < Limit) && (currentScore <  *x))
						{
							gScore.Add(neighbour, currentScore);
							roots.Add(neighbour, current);
						}
					}
					else if (currentScore < Limit)
					{
						gScore.Add(neighbour, currentScore);
						roots.Add(neighbour, current);
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("failed to find %d: %d"), current.x, current.y);
				}
			}
		}
	}

	DebugPath(gScore);

	return roots;
}

TArray<FVector2DInt> AWorldGrid::CreatePathFromRoutes(TMap<FVector2DInt, FVector2DInt> cameFrom, FVector2DInt Dest)
{
	FVector2DInt* current = &Dest;
	TArray<FVector2DInt> path = TArray<FVector2DInt>();
	path.Add(*current);
	while (cameFrom.Contains(*current))
	{
		current = cameFrom.Find(*current);
		if (current) {
			path.Add(*current);
		}
		else {
			break;
		}
	}
	path.Pop();
	Algo::Reverse(path);
	return path;
}

void AWorldGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//Empty the array and delete all it's components
	for (USceneComponent* elem : waste)
	{
		if (elem)
		{
			elem->DestroyComponent();
		}
	}
	
	waste.Empty();
	gridTiles.Empty();
	InstanceMesh->ClearInstances();

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			InstanceMesh->AddInstance(FTransform(FVector((float)(x * spacing), (float)(y * spacing), 0.0f)));

			UTextRenderComponent* text = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
			if (text) {
				text->RegisterComponent();
				waste.Add(text);
				//Attach the component to the root component
				text->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				FTransform textTrans = FTransform(FRotator(90.0f, 0.0f, 0.0f), FVector((float)(x * spacing), (float)(y * spacing), 10.0f), FVector(1.0f));
				text->SetRelativeTransform(textTrans);
			}

			UGridCollision* col = NewObject<UGridCollision>(this, UGridCollision::StaticClass());
			if (col) {
				col->RegisterComponent();
				waste.Add(col);

				col->SetBoxExtent(CollisionExtent, true);
				col->pos = FVector2DInt(x, y);
				col->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
				col->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				FTransform colTrans = FTransform(FRotator(0.0f), FVector((float)(x * spacing), (float)(y * spacing), 0.0f), FVector(1.0f));
				col->SetRelativeTransform(colTrans);
			}
			gridTiles.Add(FVector2DInt(x,y), FGridTile(true,col,text));
		}
	}

	//Register all the components
	RegisterAllComponents();
}

void AWorldGrid::DebugPath(TMap<FVector2DInt, int32> gScore)
{
	for (TPair<FVector2DInt, FGridTile>& tile : gridTiles)
	{
		tile.Value.DebugTextRender->SetText(TEXT("x"));
	}

	for (TPair<FVector2DInt, int32>& elem : gScore)
	{
		FGridTile* tile = gridTiles.Find(elem.Key);
		if (tile)
		{
			tile->DebugTextRender->SetText(FText::FromString(FString::FromInt(elem.Value)));
		}
	}
}

TArray<FVector2DInt> AWorldGrid::GetNeighbours(FVector2DInt origin)
{
	TArray<FVector2DInt> neighbours = TArray<FVector2DInt>();

	TArray<FVector2DInt> tNeighbours = TArray<FVector2DInt>();
	tNeighbours.Add(FVector2DInt(-1,  1)	+ origin);
	tNeighbours.Add(FVector2DInt( 0,  1)	+ origin);
	tNeighbours.Add(FVector2DInt( 1,  1)	+ origin);
	tNeighbours.Add(FVector2DInt(-1,  0)	+ origin);
	tNeighbours.Add(FVector2DInt( 1,  0)	+ origin);
	tNeighbours.Add(FVector2DInt(-1, -1) + origin);
	tNeighbours.Add(FVector2DInt( 0, -1)	+ origin);
	tNeighbours.Add(FVector2DInt( 1, -1)	+ origin);

	for (FVector2DInt& neighbour : tNeighbours)
	{
		if (
			(neighbour.x >= 0 && neighbour.x < width) && 
			(neighbour.y >= 0 && neighbour.y < height))
		{
			neighbours.Add(neighbour);
		}
	}
	return neighbours;
}
