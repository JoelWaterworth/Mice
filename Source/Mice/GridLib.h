// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridLib.generated.h"

USTRUCT(BlueprintType)
struct FVector2DInt
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "position")
		int32 x;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "position")
		int32 y;

	//Constructor
	FVector2DInt(int32 n_x = 0, int32 n_y = 0) : x(n_x), y(n_y) {}

	FORCEINLINE FVector2DInt operator+(const FVector2DInt& V) const {
		return FVector2DInt(x + V.x, y + V.y);
	}

	FORCEINLINE bool operator==(const FVector2DInt& V) const {
		return x == V.x && y == V.y;
	}

	FORCEINLINE FVector2DInt operator*(const int32& V) const {
		return FVector2DInt(x * V, y * V);
	}
};

FORCEINLINE uint32 GetTypeHash(const FVector2DInt& Vector)
{
	// Note: this assumes there's no padding in FVector2D that could contain uncompared data.
	return FCrc::MemCrc_DEPRECATED(&Vector, sizeof(FVector2DInt));
}

/**
 * 
 */
UCLASS()
class MICE_API UGridLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Add FVector2DInt", CompactNodeTitle = "+", Keywords = "+ add"), Category = "Math")
		static FVector2DInt Add_FVector2DIntFVector2DInt(const FVector2DInt &arg1, const FVector2DInt &arg2);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Add Int", CompactNodeTitle = "+", Keywords = "+ add"), Category = "Math")
		static FVector2DInt Add_FVector2DIntInt(const FVector2DInt &arg1, const int32 &arg2);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Mul Int", CompactNodeTitle = "*", Keywords = "* mul"), Category = "Math")
		static FVector2DInt Mul_FVector2DIntInt(const FVector2DInt &arg1, const int32 &arg2);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal FVector2DInt", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Math")
		static bool EqualEqual_FVector2DIntFVector2DInt(const FVector2DInt &arg1, const FVector2DInt &arg2);
};
