// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitSpawn.h"
#include "WorldGrid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitSpawn::AUnitSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
#if WITH_EDITOR
void AUnitSpawn::PostEditMove(bool bFinished)
{
	if (bFinished && RootComponent != nullptr && GetWorld() != nullptr)
	{
		auto loc = GetActorLocation();
		loc = loc / 100.0f;
		FIntVector pos = FIntVector(FMath::FloorToInt(loc.X), FMath::FloorToInt(loc.Y), FMath::FloorToInt(loc.Z));
		SetActorLocation(FVector(pos * 100) + FVector(50.0f, 50.0f, 0.0f));
		origin = pos;
		float x = (float)((int)GetActorRotation().Yaw % 360);
		float angle = x / 90.0f;
		auto rot = FRotator(0.0f, FMath::RoundToFloat(angle) * 90.0f, 0.0f);
		SetActorRotation(rot);
		auto vec = rot.RotateVector(FVector(0.0f, 1.0f, 0.0f));
		direction = FIntVector(
			FMath::RoundToInt(vec.X),
			FMath::RoundToInt(vec.Y),
			FMath::RoundToInt(vec.Z)
		);
	}

	AActor::PostEditMove(bFinished);
}
#endif