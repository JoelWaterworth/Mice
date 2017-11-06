// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGrid.h"
#include "Algo/Reverse.h"
#include "EngineUtils.h"
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

	TArray<FGridTransform> trans;

	for (TActorIterator<AGridObject> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Access the subclass instance with the * or -> operators.
		AGridObject* object = *ActorItr;

		trans.Add(object->GridOrigin);
	}

	for (FGridTransform& tran : trans)
	{
		for (FIntVector& pos : tran.Posistions)
		{
			FIntVector loc = pos + tran.Origin;
			int32 x = loc.X, y = loc.Y, z = loc.Z;
			FVector vec = FVector((float)(x * spacing), (float)(y * spacing), (float)(z * spacing));

			InstanceMesh->AddInstance(FTransform(vec));

			UTextRenderComponent* text = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
			if (text) {
				text->RegisterComponent();
				waste.Add(text);
				//Attach the component to the root component
				text->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				FTransform textTrans = FTransform(FRotator(90.0f, 0.0f, 0.0f), vec, FVector(1.0f));
				text->SetRelativeTransform(textTrans);
			}

			UGridCollision* col = NewObject<UGridCollision>(this, UGridCollision::StaticClass());
			if (col) {
				col->RegisterComponent();
				waste.Add(col);

				col->SetBoxExtent(CollisionExtent, true);
				col->pos = FIntVector(x, y, 0);
				col->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
				col->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				FTransform colTrans = FTransform(FRotator(0.0f), vec);
				col->SetRelativeTransform(colTrans);
			}
			gridTiles.Add(FIntVector(x, y, z), FGridTile(tran.isWalkable, col, text));
		}
	}
}

// Called every frame
void AWorldGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform AWorldGrid::VectorToWorldTransform(FIntVector pos)
{
	return VectorToLocalTransform(pos) * GetActorTransform();
}

FTransform AWorldGrid::VectorToLocalTransform(FIntVector pos)
{
	FIntVector nPos = pos * spacing;
	return FTransform(FVector((float)nPos.X, (float)nPos.Y, (float)nPos.Z));
}

FIntVector AWorldGrid::LocationToVector(FVector currentPos)
{
	FVector RelativePos = currentPos - GetActorLocation();
	FIntVector pos = FIntVector();
	pos.X = (int32)FMath::RoundHalfToZero(RelativePos.X / spacing);
	pos.Y = (int32)FMath::RoundHalfToZero(RelativePos.Y / spacing);
	pos.Z = (int32)FMath::RoundHalfToZero(RelativePos.Z / spacing);
	return pos;
}

TMap<FIntVector, FIntVector> AWorldGrid::CalculatePaths(AUnit * Unit, int32 Limit)
{
	TArray<FIntVector> OpenSet = TArray<FIntVector>();
	OpenSet.Add(Unit->pos);

	TArray<FIntVector> CloseSet = TArray<FIntVector>();

	TMap<FIntVector, int32> gScore = TMap<FIntVector, int32>();
	gScore.Add(Unit->pos, 0);
	TMap<FIntVector, FIntVector> roots = TMap<FIntVector, FIntVector>();
	while (OpenSet.Num() > 0) {
		FIntVector current = OpenSet[0];
		OpenSet.RemoveAt(0);
		CloseSet.Add(current);

		TArray<FIntVector> neighbours = GetNeighbours(current);

		for (FIntVector& neighbour : neighbours)
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
					UE_LOG(LogTemp, Warning, TEXT("failed to find %d: %d"), current.X, current.Y);
				}
			}
		}
	}

	DebugPath(gScore);

	return roots;
}

TArray<FIntVector> AWorldGrid::CreatePathFromRoutes(TMap<FIntVector, FIntVector> cameFrom, FIntVector Dest)
{
	FIntVector* current = &Dest;
	TArray<FIntVector> path = TArray<FIntVector>();
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
	/*
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
				col->pos = FIntVector(x, y, 0);
				col->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
				col->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				FTransform colTrans = FTransform(FRotator(0.0f), FVector((float)(x * spacing), (float)(y * spacing), 0.0f), FVector(1.0f));
				col->SetRelativeTransform(colTrans);
			}
			gridTiles.Add(FIntVector(x,y, 0), FGridTile(true,col,text));
		}
	}
	*/

	//Register all the components
	RegisterAllComponents();
}

void AWorldGrid::DebugPath(TMap<FIntVector, int32> gScore)
{
	for (TPair<FIntVector, FGridTile>& tile : gridTiles)
	{
		tile.Value.DebugTextRender->SetText(TEXT("x"));
	}

	for (TPair<FIntVector, int32>& elem : gScore)
	{
		FGridTile* tile = gridTiles.Find(elem.Key);
		if (tile)
		{
			tile->DebugTextRender->SetText(FText::FromString(FString::FromInt(elem.Value)));
		}
	}
}

TArray<FIntVector> AWorldGrid::GetNeighbours(FIntVector origin)
{
	TArray<FIntVector> neighbours = TArray<FIntVector>();

	TArray<FIntVector> tNeighbours = TArray<FIntVector>();
	tNeighbours.Add(FIntVector(-1,  1, 0)	+ origin);
	tNeighbours.Add(FIntVector( 0,  1, 0)	+ origin);
	tNeighbours.Add(FIntVector( 1,  1, 0)	+ origin);
	tNeighbours.Add(FIntVector(-1,  0, 0)	+ origin);
	tNeighbours.Add(FIntVector( 1,  0, 0)	+ origin);
	tNeighbours.Add(FIntVector(-1, -1, 0)	+ origin);
	tNeighbours.Add(FIntVector( 0, -1, 0)	+ origin);
	tNeighbours.Add(FIntVector( 1, -1, 0)	+ origin);

	for (FIntVector& neighbour : tNeighbours)
	{
		if (
			(neighbour.X >= 0 && neighbour.X < width) && 
			(neighbour.Y >= 0 && neighbour.Y < height))
		{
			neighbours.Add(neighbour);
		}
	}
	return neighbours;
}
