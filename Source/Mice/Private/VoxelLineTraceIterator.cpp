// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelLineTraceIterator.h"
#include <limits>
#include <math.h>

FIteratorReturn UVoxelLineTraceIterator::next()
{
	auto fx = [](float& t, float direction, float start)  {
		float a = FMath::FloorToFloat(start);
		if (direction > 0.0) {
			a += 1.0;
		}
		else if (start == a) {
			a -= 1.0;
		}
		if (direction != 0.0f) {
			t = FMath::Min(t, (a - start) / direction);
		}
	};

	float t = std::numeric_limits<float>::infinity();

	fx(t, direction.X, start.X);
	fx(t, direction.Y, start.Y);
	fx(t, direction.Z, start.Z);

	FVector out = start * direction * t;
	TArray<FVoxelResult> voxels = TArray<FVoxelResult>();
	TArray<EDirection> dir = TArray<EDirection>();

	if (FMath::FloorToFloat(out.X) == out.X) {
		TArray<EDirection> d = TArray<EDirection>();
		d.Push(out.X > 0 ? EDirection::D_Rightward : EDirection::D_Leftward);
		voxels.Push(
			FVoxelResult(FIntVector(out.X - 1, out.Y, out.Z),
				d
			)
		);
		dir.Push(out.X < 0 ? EDirection::D_Rightward : EDirection::D_Leftward);
	}
	if (FMath::FloorToFloat(out.Y) == out.Y) {
		TArray<EDirection> d = TArray<EDirection>();
		d.Push(out.Y > 0 ? EDirection::D_Forward : EDirection::D_Backward);
		voxels.Push(
			FVoxelResult(FIntVector(out.X, out.Y - 1, out.Z),
				d
			)
		);
		dir.Push(out.Y < 0 ? EDirection::D_Forward : EDirection::D_Backward);
	}
	if (FMath::FloorToFloat(out.Z) == out.Z) {
		TArray<EDirection> d = TArray<EDirection>();
		d.Push(out.Z > 0 ? EDirection::D_Upward : EDirection::D_Downward);
		voxels.Push(
			FVoxelResult(FIntVector(out.X, out.Y, out.Z - 1),
				d
			)
		);
		dir.Push(out.Z < 0 ? EDirection::D_Upward : EDirection::D_Downward);
	}
	voxels.Push(FVoxelResult(
		FIntVector(out.X, out.Y, out.Z),
		dir
	));
	return FIteratorReturn(out, voxels);
}